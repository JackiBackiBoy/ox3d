#pragma once

#include "oxcart/core/core.hpp"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

namespace ox {
  class OX_API Mouse {
    public:
      // Prevent copying or moving
      Mouse(const Mouse&) = delete;
      Mouse(Mouse&&) = delete;
      Mouse& operator=(const Mouse&) = delete;
      Mouse& operator=(Mouse&&) = delete;

      static bool isButtonDown(const int& button, GLFWwindow* window);
      static glm::vec2 getPosition();
      static float getVerticalScroll();

      // Callbacks
      static void onMouseMove(GLFWwindow* window, double xpos, double ypos);
      static void onMouseScroll(GLFWwindow* window, double xoffset, double yoffset);

    private:
      Mouse() {};
      ~Mouse() {};

      static float m_PosX;
      static float m_PosY;
      static float m_ScrollX;
      static float m_ScrollY;
  };
}
