#pragma once

#include "core/core.hpp"
#include "component.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace ox {
  class OX_API Transform : public Component {
    public:
      Transform() : Component() {};
      void onStart() override {};
      void onUpdate(const float& deltaTime) override {};

      glm::mat4 mat4() {
        const float c3 = glm::cos(rotation.z);
        const float s3 = glm::sin(rotation.z);
        const float c2 = glm::cos(rotation.x);
        const float s2 = glm::sin(rotation.x);
        const float c1 = glm::cos(rotation.y);
        const float s1 = glm::sin(rotation.y);
        return glm::mat4{
          {
            scale.x * (c1 * c3 + s1 * s2 * s3),
            scale.x * (c2 * s3),
            scale.x * (c1 * s2 * s3 - c3 * s1),
            0.0f,
          },
          {
            scale.y * (c3 * s1 * s2 - c1 * s3),
            scale.y * (c2 * c3),
            scale.y * (c1 * c3 * s2 + s1 * s3),
            0.0f,
          },
          {
            scale.z * (c2 * s1),
            scale.z * (-s2),
            scale.z * (c1 * c2),
            0.0f,
          },
          {position.x, position.y, position.z, 1.0f}};
      }

      glm::vec3 position{0.0f, 0.0f, 0.0f};
      glm::vec3 scale{1.0f, 1.0f, 1.0f};
      glm::vec3 rotation{};
  };
}
