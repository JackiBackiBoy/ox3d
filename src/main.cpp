#include <iostream>

#include "core/window.h"
#include "GLFW/glfw3.h"
#include <vector>

int main() {
  //Create window
  Window::currentWindow = new Window(500, 500, "ox3d");

  try {
    Window::currentWindow->run();
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  glfwInit();

  return 0;
}
