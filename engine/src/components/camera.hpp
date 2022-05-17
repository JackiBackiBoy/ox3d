#pragma once

#include "core/core.hpp"
#include "component.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace ox {
  class OX_API Camera : public Component {
    public:
      Camera() : Component() {};

      void onStart() override {};
      void onUpdate(const float& deltaTime) override {};

      void setPerspective(
          const float& fov,
          const float& aspectRatio,
          const float& near,
          const float& far);

      inline const glm::mat4& getProjection() const { return m_ProjectionMatrix; }
      inline const glm::mat4& getView() const { return m_ViewMatrix; }

      // Getters
      inline glm::vec3 getForward() const { return m_Forward; }
      inline glm::vec3 getUp() const { return m_Up; }
      inline float getYaw() const { return m_Yaw; }
      inline float getPitch() const { return m_Pitch; }
      inline float getRoll() const { return m_Roll; }

      // Setters
      inline void setYaw(const float& yaw) { m_Yaw = yaw; }
      inline void setPitch(const float& pitch) { m_Pitch = pitch; }
      inline void setRoll(const float& roll) { m_Roll = roll; }

      glm::vec3 rotation{};
      glm::mat4 m_ProjectionMatrix{1.0f};
      glm::mat4 m_ViewMatrix{1.0f};

    private:
      glm::vec3 m_Forward{0.0f, 0.0f, 1.0f};
      glm::vec3 m_Up{0.0f, -1.0f, 0.0f};

      float m_Yaw{glm::pi<float>() / 2.0f};
      float m_Pitch{0.0f};
      float m_Roll{0.0f};
  };
}
