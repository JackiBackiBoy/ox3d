#include "camera.h"
#include <cmath>
#include <glm/gtc/constants.hpp>
#include <iostream>
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 Camera::getRight() {
  return glm::normalize(glm::cross(m_Forward, m_Up));
}

void Camera::update() {
  // Update view-related attributes if necessary
  if (m_UpdateView) {
    m_ViewMatrix = glm::lookAt(m_Position, // eye
                               m_Position + m_Forward, // center
                               m_Up); // up

    m_UpdateView = false;
  }
}

void Camera::setPosition(const glm::vec3& position) {
  glm::vec3 prevPos = m_Position;
  m_Position = position;

  if (m_Position != prevPos) { // only request update if position has changed
    m_UpdateView = true;
  }
}

void Camera::setYaw(const float& degrees) {
  float prevYaw = m_Yaw;
  m_Yaw = fmod(degrees, 360.0f);

  if (m_Yaw != prevYaw) {
    // Euler angles direction vector
    m_Forward.x = -cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Forward.z = -sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Forward = glm::normalize(m_Forward);
    
    m_UpdateView = true;
  }
}

void Camera::setPitch(const float& degrees) {
  float prevPitch;
  m_Pitch = degrees;

  // TODO: Allow full pitch rotations
  if (m_Pitch >= 90.0f) {
    m_Pitch = 89.999f;
  }
  else if (m_Pitch <= -90.f) {
    m_Pitch = -89.999f;
  }

  if (m_Pitch != prevPitch) {
    // Euler angles direction vector
    m_Forward.x = -cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Forward.y = -sin(glm::radians(m_Pitch));
    m_Forward.z = -sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Forward = glm::normalize(m_Forward);
    
    m_UpdateView = true;
  }
}

void Camera::setFOV(const float& degrees) {
  m_FOV = degrees;

  // Restrictions for field of view angles
  if (m_FOV > 120.0f) {
    m_FOV = 120.0f;
  }
  else if (m_FOV < 10.0f) {
    m_FOV = 10.0f;
  }
}
