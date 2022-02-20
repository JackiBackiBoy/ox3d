#include <iostream>

#include "core/window.h"
#include "GLFW/glfw3.h"
#include <vector>

int main() {
  //Create window
  Window* window = new Window(500, 500, "ox3d");
  window->run();

  // Delete window
  window = nullptr;
  delete window;

  std::cout << "Warming up Vulkan graphics engine..." << std::endl;
  std::vector<int> extensions(5);
  glfwInit();

  return 0;
}
