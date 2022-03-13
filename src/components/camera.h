#pragma once

#include <glm/glm.hpp>

class Camera {
  public:
    Camera()
      : m_Position({ 0.0f, 0.0f, 0.0f }),
      m_Forward({ 0.0f, 0.0f, -1.0f }),
      m_Up({ 0.0f, 1.0f, 0.0f }) {};
    ~Camera() {};

    glm::vec3 getRight();

    // Getters
    inline glm::vec3& getPosition() { return m_Position; }
    inline glm::vec3 getForward() { return m_Forward; }
    inline glm::vec3 getUp() { return m_Up; }
    inline float getYaw() { return m_Yaw; }

    // Setters
    void setYaw(const float& angle);
    inline void setPitch(const float& angle);

  private:
    glm::vec3 m_Position;
    glm::vec3 m_Forward;
    glm::vec3 m_Up;

    // Euler angles
    float m_Yaw;
    float m_Pitch;
};
