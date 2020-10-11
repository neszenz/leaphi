#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

// public interface  +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +

Camera::Camera(float fov, float aspect, float near, float far) : m_fov(fov), m_aspect(aspect), m_near(near), m_far(far) {
}

glm::mat4 Camera::view_matrix() const {
    return glm::affineInverse(this->model_matrix());
}
glm::mat4 Camera::proj_matrix() const {
    return glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
}

glm::vec3 Camera::front() const {
    return glm::vec3(0.0f, 0.0f, -1.0f) * this->orientation();
}
glm::vec3 Camera::right() const {
    return glm::vec3(1.0f, 0.0f, 0.0f) * this->orientation();
}
glm::vec3 Camera::up() const {
    return glm::vec3(0.0f, 1.0f, 0.0f) * this->orientation();
}

void Camera::move_forward(float value) {
    this->translate(value * this->front());
}
void Camera::move_backward(float value) {
    this->move_forward(-value);
}
void Camera::move_right(float value) {
    this->translate(value * this->right());
}
void Camera::move_left(float value) {
    this->move_right(-value);
}
void Camera::move_up(float value) {
    this->translate(value * this->up());
}
void Camera::move_down(float value) {
    this->move_up(-value);
}

void Camera::set_aspect(float aspect) {
    m_aspect = aspect;
}
