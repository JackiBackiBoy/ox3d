#include "oxcart/input/mouse.hpp"

namespace ox {
  bool Mouse::isButtonDown(const int& button, GLFWwindow* window) {
    return glfwGetMouseButton(window, button);
  }

  glm::vec2 Mouse::getPosition() {
    return { m_PosX, m_PosY };
  }

  void Mouse::setPosition(const glm::vec2& pos, GLFWwindow* window) {
    glfwSetCursorPos(window, pos.x, pos.y);
  }

  float Mouse::getVerticalScroll() {
    float value = m_ScrollY;
    m_ScrollY = 0.0f;
    return value;
  }

  void Mouse::onMouseMove(GLFWwindow* window, double xpos, double ypos) {
    m_PosX = float(xpos);
    m_PosY = float(ypos);
  }

  void Mouse::onMouseScroll(GLFWwindow* window, double xoffset, double yoffset) {
    m_ScrollX = float(xoffset);
    m_ScrollY = float(yoffset);
  }

  float Mouse::m_PosX = 0.0f;
  float Mouse::m_PosY = 0.0f;
  float Mouse::m_ScrollX = 0.0f;
  float Mouse::m_ScrollY = 0.0f;
}
