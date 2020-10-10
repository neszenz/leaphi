#include <geometry.hpp>

#include <assert.h>
#include <iostream>
#include <limits>
#include <utility>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "util.hpp"

#define INF std::numeric_limits<float>::infinity()
#define DEFAULT_AABB {INF, -INF, INF, -INF, INF, -INF}

const std::vector<float> default_v_buffer = {
    // color cube vertices
    // position             // color
    -1.0f, -1.0f, -1.0f,    0.0f, 0.0f, 0.0f, // 0 back  left  bottom
     1.0f, -1.0f, -1.0f,    1.0f, 0.0f, 0.0f, // 1 back  right bottom
     1.0f,  1.0f, -1.0f,    1.0f, 1.0f, 0.0f, // 2 back  right up
    -1.0f,  1.0f, -1.0f,    0.0f, 1.0f, 0.0f, // 3 back  left  up
    -1.0f, -1.0f,  1.0f,    0.0f, 0.0f, 1.0f, // 4 front left  bottom
     1.0f, -1.0f,  1.0f,    1.0f, 0.0f, 1.0f, // 5 front right bottom
     1.0f,  1.0f,  1.0f,    1.0f, 1.0f, 1.0f, // 6 front right up
    -1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 1.0f  // 7 front left  up
};
const std::vector<unsigned> default_e_buffer = {
    // color cube indices
    4, 5, 6, 4, 6, 7, // front
    0, 2, 1, 0, 3, 2, // back
    0, 4, 7, 0, 7, 3, // left
    5, 1, 2, 6, 5, 2, // right
    3, 7, 2, 2, 7, 6, // top
    4, 0, 1, 4, 1, 5, // bottom
};

// public interface  +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +

Geometry::Geometry(const v_buffer_t& v_buffer, const e_buffer_t& e_buffer) : m_v_buffer(v_buffer), m_e_buffer(e_buffer) {
    m_bounding_box = this->compute_bounding_box(m_v_buffer);
    m_gbi = this->create_geometry_buffer(m_v_buffer, m_e_buffer);
}
Geometry::Geometry() : Geometry(default_v_buffer, default_e_buffer) {
}

Geometry::Geometry(const Geometry& other) : Geometry(other.m_v_buffer, other.m_e_buffer) {
}
Geometry& Geometry::operator=(Geometry other) {
    swap(*this, other);
    return *this;
}
Geometry::Geometry(Geometry&& other) {
    m_bounding_box = std::move(other.m_bounding_box);
    m_gbi = std::exchange(other.m_gbi, {0, 0, 0});
    m_v_buffer = std::move(other.m_v_buffer);
    m_e_buffer = std::move(other.m_e_buffer);
}
Geometry& Geometry::operator=(Geometry&& other) {
    m_bounding_box = std::move(other.m_bounding_box);
    m_gbi = std::exchange(other.m_gbi, {0, 0, 0});
    m_v_buffer = std::move(other.m_v_buffer);
    m_e_buffer = std::move(other.m_e_buffer);

    return *this;
}
Geometry::~Geometry() {
    GL(glDeleteBuffers(1, &m_gbi.ebo));
    GL(glDeleteBuffers(1, &m_gbi.vbo));
    GL(glDeleteVertexArrays(1, &m_gbi.vao));
}

void swap(Geometry& first, Geometry& second) {
    using std::swap; // enables ADL

    swap(first.m_bounding_box, second.m_bounding_box);
    swap(first.m_gbi, second.m_gbi);
    swap(first.m_v_buffer, second.m_v_buffer);
    swap(first.m_e_buffer, second.m_e_buffer);
}

void Geometry::draw() const {
    GL(glBindVertexArray(m_gbi.vao));
    GL(glDrawElements(GL_TRIANGLES, m_gbi.n_elements, GL_UNSIGNED_INT, NULL));
    GL(glBindVertexArray(0));
}

// private interface +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +

Geometry::aabb_t Geometry::compute_bounding_box(const v_buffer_t& v_buffer) {
    const unsigned alignment = 3;
    assert(v_buffer.size() % alignment == 0);

    aabb_t bounding_box = DEFAULT_AABB;

    for (unsigned i = 0; i+alignment <= v_buffer.size(); i += alignment) {
        float v_x = v_buffer.at(i+0);
        float v_y = v_buffer.at(i+1);
        float v_z = v_buffer.at(i+2);

        if (v_x < m_bounding_box.x_min)
            bounding_box.x_min = v_x;
        if (v_x < m_bounding_box.x_max)
            bounding_box.x_max = v_x;

        if (v_y < m_bounding_box.y_min)
            bounding_box.y_min = v_y;
        if (v_y < m_bounding_box.y_max)
            bounding_box.y_max = v_y;

        if (v_z < m_bounding_box.z_min)
            bounding_box.z_min = v_z;
        if (v_z < m_bounding_box.z_max)
            bounding_box.z_max = v_z;
    }

    return bounding_box;
}

Geometry::gbi_t Geometry::create_geometry_buffer(const v_buffer_t& v_buffer,
                                                 const e_buffer_t& e_buffer) {
    unsigned v_size = sizeof(v_buffer[0]) * v_buffer.size();
    unsigned e_size = sizeof(e_buffer[0]) * e_buffer.size();

    unsigned vao;
    GL(glGenVertexArrays(1, &vao));
    GL(glBindVertexArray(vao));

    unsigned vbo;
    GL(glGenBuffers(1, &vbo));
    GL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL(glBufferData(GL_ARRAY_BUFFER, v_size, &v_buffer[0], GL_STATIC_DRAW));
    GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void *)0));
    GL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void *)(3*sizeof(float))));
    GL(glEnableVertexAttribArray(0));
    GL(glEnableVertexAttribArray(1));

    unsigned ebo;
    GL(glGenBuffers(1, &ebo));
    GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, e_size, &e_buffer[0], GL_STATIC_DRAW));

    GL(glBindVertexArray(0));
    GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    return {vao, vbo, ebo, e_buffer.size()};
}
