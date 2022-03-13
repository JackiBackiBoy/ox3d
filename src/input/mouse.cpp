#include "mouse.h"
#include <GLFW/glfw3.h>
#include "core/window.h"

bool Mouse::isButtonDown(const int& button) {
  return glfwGetMouseButton(Window::currentWindow->getRawWindow(), button);
}

glm::vec2 Mouse::getPosition() {
  double posX;
  double posY;

  glfwGetCursorPos(Window::currentWindow->getRawWindow(), &posX, &posY);

  return { static_cast<float>(posX), static_cast<float>(posY) };
}
