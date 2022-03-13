#include "camera.h"
#include <cmath>

glm::vec3 Camera::getRight() {
  glm::vec3 up = { 0.0f, 1.0f, 0.0f };

  return glm::normalize(glm::cross(m_Forward, up));
}

void Camera::setYaw(const float& angle) {
  m_Yaw = angle;

  m_Forward = { sin(angle), m_Forward.y, -cos(angle) };
  m_Forward = glm::normalize(m_Forward);
}

void Camera::setPitch(const float& angle) {

}
