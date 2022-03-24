#pragma once

#include "core/core.h"
#include <glm/glm.hpp>

namespace ox {
  class OX_API Camera {
    public:
      Camera()
        : m_Position({ 0.0f, 0.0f, 0.0f }),
        m_Up({ 0.0f, 1.0f, 0.0f }),
        m_Yaw(0.0f),
        m_Pitch(0.0f),
        m_Roll(0.0f),
        m_FOV(45.0f),
        m_UpdateView(true) {};
      ~Camera() {};

      void update();

      // Getters
      glm::vec3 getRight();
      inline glm::vec3 getPosition() { return m_Position; }
      inline glm::vec3 getForward() { return m_Forward; }
      inline glm::vec3 getUp() { return m_Up; }
      inline float getYaw() { return m_Yaw; }
      inline float getPitch() { return m_Pitch; }
      inline glm::mat4 getViewMatrix() { return m_ViewMatrix; }
      inline float getFOV() { return m_FOV; }

      // Setters
      void setPosition(const glm::vec3& position);
      void setYaw(const float& degrees);
      void setPitch(const float& degrees);
      void setFOV(const float& degrees);

    private:
      glm::vec3 m_Position;
      glm::vec3 m_Forward;
      glm::vec3 m_Up;
      glm::mat4 m_ViewMatrix;

      // Euler angles
      float m_Yaw;
      float m_Pitch;
      float m_Roll;

      float m_FOV;
      bool m_UpdateView;
  };
}
