#include "keyboard.h"
#include "core/window.h"
#include <GLFW/glfw3.h>

using namespace ox;

bool Keyboard::isKeyDown(const KeyCode& key) {
  return glfwGetKey(Window::currentWindow->getRawWindow(), (int)key) == GLFW_PRESS;
}
