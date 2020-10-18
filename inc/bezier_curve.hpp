#pragma once

#include "geometry.hpp"
#include "mesh.hpp"
#include "shader.hpp"

#define N_SAMPLES 32

class Bezier_Curve {
    public:
        Bezier_Curve(std::vector<glm::vec3> points = std::vector<glm::vec3>());

        void add(glm::vec3 point);

        Mesh sample_mesh_until(float p_limit, unsigned n_samples=N_SAMPLES) const;
        Mesh sample_mesh(unsigned n_samples=N_SAMPLES) const;

    private:
        std::vector<glm::vec3> m_points;
};
