#include "camera.hpp"

#include <cassert>
#include <limits>

namespace ox {
  void Camera::setPerspective(
      const float& fov,
      const float& aspect,
      const float& near,
      const float& far) {
    m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspect, near, far);
  }
}
