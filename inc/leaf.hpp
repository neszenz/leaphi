#pragma once

#include "mesh.hpp"

class Leaf {
    public:
        // f: generation factor [-1,1] and size_factor: size of fully grown leaf
        Leaf(float f, float size_factor = 1.0f);

        // leaf mesh at growth stage g [0,1]
        Mesh mesh(float growth_stage = 1.0f) const;
    private:
        float m_f;
        float m_size_factor;
};
