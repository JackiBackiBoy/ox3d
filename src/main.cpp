#include <iostream>

#include "core/window.h"
#include "GLFW/glfw3.h"
#include <vector>


int main() {
  //Create window
  ox::Window::currentWindow = new ox::Window(1280, 720, "ox3d");

  try {
    ox::Window::currentWindow->run();
  }
  catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  glfwInit();

  return 0;
}
