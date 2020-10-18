#pragma once

#include "mesh.hpp"

class Leaf {
    public:
        // f: generation factor [-1,1] and v: variance limiter {2...}
        Leaf(float f, int variance_limiter = 16, float size = 1.0f);

        // leaf mesh at growth stage g [0,1]
        Mesh mesh(float growth_stage = 1.0f) const;
    private:
        float m_f;
        float m_size;
        float m_variance_limiter;
};
