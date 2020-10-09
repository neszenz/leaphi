#include <geometry.hpp>

#include <assert.h>
#include <iostream>
#include <limits>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "util.hpp"

#define INF std::numeric_limits<float>::infinity()
#define DEFAULT_AABB {INF, -INF, INF, -INF, INF, -INF}

const std::vector<float> default_v_buffer = {
    -0.8f, -0.8f, -0.1f,   1.0f, 0.0f, 0.0f,
     0.8f, -0.8f, -0.1f,   0.0f, 1.0f, 0.0f,
     0.0f,  0.8f, -0.1f,   0.0f, 0.0f, 1.0f
};
const std::vector<unsigned> default_e_buffer = {
    0, 1, 2
};

// public interface  +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +

Geometry::Geometry(const v_buffer_t& v_buffer, const e_buffer_t& e_buffer) {
    m_bounding_box = this->compute_bounding_box(v_buffer);
    m_gbi = this->create_geometry_buffer(v_buffer, e_buffer);
}

Geometry::Geometry() : Geometry(default_v_buffer, default_e_buffer) {
}

Geometry::~Geometry() {
    GL(glDeleteBuffers(1, &m_gbi.ebo));
    GL(glDeleteBuffers(1, &m_gbi.vbo));
    GL(glDeleteVertexArrays(1, &m_gbi.vao));
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
