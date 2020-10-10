#include "mesh.hpp"

// public interface  +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +

Mesh::Mesh(Geometry geometry, const Shader& shader) : m_geometry(std::move(geometry)), m_shader(std::move(shader)) {
}

const Shader& Mesh::shader() const {
    return m_shader;
}

void Mesh::draw() const {
    m_shader.use();
    m_geometry.draw();
}
