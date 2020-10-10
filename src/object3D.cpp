#include "object3D.hpp"

#include <glm/gtc/matrix_transform.hpp>

// public interface  +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ - +=+ +

Object3D::Object3D() : m_model(glm::mat4(1.0f)) {
}

glm::mat4 Object3D::model_matrix() const {
    return m_model;
}

void Object3D::translate(glm::vec3 value) {
    m_model = glm::translate(m_model, value);
}

void Object3D::rotate(float value, glm::vec3 axis) {
    m_model = glm::rotate(m_model, value, axis);
}
