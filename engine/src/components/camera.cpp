#include "camera.hpp"

#include <cassert>
#include <limits>

namespace ox {
  void Camera::setPerspective(
      const float& fov,
      const float& aspect,
      const float& near,
      const float& far) {
    m_ProjectionMatrix = glm::perspective(fov, aspect, near, far);
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
}
