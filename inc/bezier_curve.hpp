#pragma once

#include "geometry.hpp"
#include "mesh.hpp"
#include "shader.hpp"

class Bezier_Curve {
    public:
        Bezier_Curve(std::vector<glm::vec3> points = std::vector<glm::vec3>());

        void add(glm::vec3 point);

        Mesh sample_mesh(unsigned n_samples = 32) const;

    private:
        std::vector<glm::vec3> m_points;
};
