#include "keyboard.hpp"

namespace ox {
  bool Keyboard::isKeyDown(const Keycode& key, Window& window) {
    return glfwGetKey(window.getRawWindow(), (int)key) == GLFW_PRESS;
  }
}
