#include "oxcart/input/mouse.hpp"

namespace ox {
  bool Mouse::isButtonDown(const int& button, GLFWwindow* window) {
    return glfwGetMouseButton(window, button);
  }

  glm::vec2 Mouse::getPosition() {
    return { posX, posY };
  }

  float Mouse::getVerticalScroll() {
    float value = scrollY;
    scrollY = 0.0f;
    return value;
  }

  void Mouse::onMouseMove(GLFWwindow* window, double xpos, double ypos) {
    posX = float(xpos);
    posY = float(ypos);
  }

  void Mouse::onMouseScroll(GLFWwindow* window, double xoffset, double yoffset) {
    scrollX = float(xoffset);
    scrollY = float(yoffset);
  }

  float Mouse::posX = 0.0f;
  float Mouse::posY = 0.0f;
  float Mouse::scrollX = 0.0f;
  float Mouse::scrollY = 0.0f;
}
