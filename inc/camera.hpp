#pragma once

#include "object3D.hpp"

class Camera : public Object3D {
    public:
        Camera(float fov, float aspect, float near = 0.1f, float far = 1000.0f);

        glm::mat4 view_matrix() const;
        glm::mat4 proj_matrix() const;

        glm::vec3 front() const;
        glm::vec3 right() const;
        glm::vec3 up() const;

        void move_forward(float value);
        void move_backward(float value);
        void move_right(float value);
        void move_left(float value);
        void move_up(float value);
        void move_down(float value);

        void set_aspect(float aspect);
    private:
        float m_fov;
        float m_aspect;
        float m_near;
        float m_far;
};
