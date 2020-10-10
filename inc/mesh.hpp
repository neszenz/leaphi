#pragma once

#include "geometry.hpp"
#include "object3D.hpp"
#include "shader.hpp"

class Mesh : public Object3D {
    public:
        Mesh(Geometry geometry, const Shader& shader);

        Shader& shader();

        void draw() const;
    private:
        Geometry m_geometry;
        Shader m_shader;
};