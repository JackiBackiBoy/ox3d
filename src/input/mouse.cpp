#include "mouse.h"
#include "core/window.h"

bool Mouse::isButtonDown(const int& button) {
  return glfwGetMouseButton(Window::currentWindow->getRawWindow(), button);
}

glm::vec2 Mouse::getPosition() {
  return { posX, posY };
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

double Mouse::posX = 0.0;
double Mouse::posY = 0.0;
bool Mouse::hasMoved = false;
bool Mouse::firstMove = false;
bool Mouse::lastMoved = false;
