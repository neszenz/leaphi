#pragma once

#include <glm/glm.hpp>

class Object3D {
    public:
        Object3D();

        glm::mat4 model_matrix() const;

        void translate(glm::vec3 value);

        void rotate(float value, glm::vec3 axis);
    private:
        glm::mat4 m_model;
};
