#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

// public interface  +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +

Camera::Camera(float fov, float aspect, float near, float far) : m_fov(fov), m_aspect(aspect), m_near(near), m_far(far) {
}

void Camera::set_aspect(float aspect) {
    m_aspect = aspect;
}

glm::mat4 Camera::view_matrix() const {
    return glm::affineInverse(this->model_matrix());
}

glm::mat4 Camera::proj_matrix() const {
    return glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
}
