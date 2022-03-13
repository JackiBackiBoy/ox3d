#include "mouse.h"
#include "core/window.h"

bool Mouse::isButtonDown(const int& button) {
  return glfwGetMouseButton(Window::currentWindow->getRawWindow(), button);
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
  if (!hasMoved) {
    hasMoved = true;
  }

  if (!lastMoved && hasMoved) {
    lastMoved = true;
    firstMove = true;
  }
  else {
    firstMove = false;
  }

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
bool Mouse::hasMoved = false;
bool Mouse::firstMove = false;
bool Mouse::lastMoved = false;
