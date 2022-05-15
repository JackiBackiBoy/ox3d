#pragma once

#include "core/core.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace ox {
  class OX_API Camera {
    public:
      void setOrthographic(
          const float& left,
          const float& right,
          const float& top,
          const float& bottom,
          const float& near,
          const float& far);

      void setPerspective(
          const float& verticalFOV,
          const float& aspectRatio,
          const float& near,
          const float& far);

      void setDirection(
          glm::vec3 position,
          glm::vec3 direction,
          glm::vec3 up = { 0.0f, -1.0f, 0.0f });

      void setTarget(
          glm::vec3 position,
          glm::vec3 target,
          glm::vec3 up = { 0.0f, -1.0f, 0.0f });

      void setYXZ(glm::vec3 position, glm::vec3 rotation);

      inline const glm::mat4& getProjection() const { return m_ProjectionMatrix; }
      inline const glm::mat4& getView() const { return m_ViewMatrix; }
    private:
      glm::mat4 m_ProjectionMatrix{1.0f};
      glm::mat4 m_ViewMatrix{1.0f};
  };
}
