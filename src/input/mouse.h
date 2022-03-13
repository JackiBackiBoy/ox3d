#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

// Singleton
class Mouse {
  public:
    // Prevent copying or moving
    Mouse(const Mouse&) = delete;
    Mouse(Mouse&&) = delete;
    Mouse& operator=(const Mouse&) = delete;
    Mouse& operator=(Mouse&&) = delete;

    static bool isButtonDown(const int& button);
    static glm::vec2 getPosition();
    static float getVerticalScroll();

    // Callbacks
    static void onMouseMove(GLFWwindow* window, double xpos, double ypos);
    static void onMouseScroll(GLFWwindow* window, double xoffset, double yoffset);

    static bool hasMoved;
    static bool firstMove;

  private:
    Mouse() {};
    ~Mouse() {};

    static float posX;
    static float posY;
    static float scrollX;
    static float scrollY;
    static bool lastMoved;
};
