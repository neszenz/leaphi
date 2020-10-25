#pragma once

#include "geometry.hpp"
#include "object3D.hpp"
#include "shader.hpp"

class Mesh : public Object3D {
    public:
        Mesh(Geometry geometry, const Shader& shader);

        friend Mesh operator+(const Mesh& first, const Mesh& second);

        const Shader& shader() const;

        v_buffer_t get_vertices() const;

        void draw() const;
    private:
        Geometry m_geometry;
        Shader m_shader;
};
