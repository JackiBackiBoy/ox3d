#include "camera.h"
#include <cmath>
#include <glm/gtc/constants.hpp>
#include <iostream>
#include <math.h>

glm::vec3 Camera::getRight() {
  glm::vec3 up = { 0.0f, 1.0f, 0.0f };

  return glm::normalize(glm::cross(m_Forward, up));
}

void Camera::setYaw(const float& angle) {
  m_Yaw = angle;
}

void Camera::setPitch(const float& angle) {
  m_Pitch = angle;

  // Euler angles direction vector
  m_Forward.x = -cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
  m_Forward.y = -sin(glm::radians(m_Pitch));
  m_Forward.z = -sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
  m_Forward = glm::normalize(m_Forward);
}
