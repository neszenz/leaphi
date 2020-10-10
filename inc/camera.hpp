#pragma once

#include "object3D.hpp"

class Camera : public Object3D {
    public:
        Camera(float fov, float aspect, float near = 0.1f, float far = 1000.0f);

        void set_aspect(float aspect);

        glm::mat4 view_matrix() const;
        glm::mat4 proj_matrix() const;
    private:
        float m_fov;
        float m_aspect;
        float m_near;
        float m_far;
};
