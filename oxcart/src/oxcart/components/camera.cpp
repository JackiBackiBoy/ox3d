#include "oxcart/components/camera.hpp"

#include <cassert>
#include <limits>

namespace ox {
  Camera::Camera() : Component() {}

  void Camera::setPerspective(
      const float& fov,
      const float& aspect,
      const float& near,
      const float& far) {
    m_ProjectionMatrix = glm::perspective(fov, aspect, near, far);
  }

  const glm::mat4& Camera::getProjection() const {
    return m_ProjectionMatrix;
  }

  const glm::mat4& Camera::getView() const {
    return m_ViewMatrix;
  }

  const glm::mat4& Camera::getInverseView() const
  {
    return m_InverseViewMatrix;
  }

  void Camera::setYaw(const float& yaw) {
    m_Yaw = glm::mod(yaw, glm::two_pi<float>());
  }

  void Camera::setPitch(const float& pitch) {
    m_Pitch = glm::clamp(pitch, -1.57f, 1.57f);
  }

  void Camera::setRoll(const float& roll) {
    m_Roll = glm::mod(roll, glm::two_pi<float>());
  }

  void Camera::onUpdate(glm::vec3 position)
  {
    const float c3 = glm::cos(rotation.z);
    const float s3 = glm::sin(rotation.z);
    const float c2 = glm::cos(rotation.x);
    const float s2 = glm::sin(rotation.x);
    const float c1 = glm::cos(rotation.y);
    const float s1 = glm::sin(rotation.y);
    const glm::vec3 u{ (c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1) };
    const glm::vec3 v{ (c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3) };
    const glm::vec3 w{ (c2 * s1), (-s2), (c1 * c2) };

    m_InverseViewMatrix = glm::mat4(1.0f);
    m_InverseViewMatrix[0][0] = u.x;
    m_InverseViewMatrix[0][1] = u.y;
    m_InverseViewMatrix[0][2] = u.z;
    m_InverseViewMatrix[1][0] = v.x;
    m_InverseViewMatrix[1][1] = v.y;
    m_InverseViewMatrix[1][2] = v.z;
    m_InverseViewMatrix[2][0] = w.x;
    m_InverseViewMatrix[2][1] = w.y;
    m_InverseViewMatrix[2][2] = w.z;
    m_InverseViewMatrix[3][0] = position.x;
    m_InverseViewMatrix[3][1] = position.y;
    m_InverseViewMatrix[3][2] = position.z;
  }

  Camera* Camera::current = nullptr;
}
