#include "window.h"
#include <iostream>

void Window::createWindow() {
  glfwInit(); // initialize glfw

  // set GLFW options
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  rawWindow = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
  
  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

  std::cout << extensionCount << " extensions supported\n";
}

void Window::run() {
  createWindow();

  // Rendering loop
  while (!glfwWindowShouldClose(rawWindow)) {
    glfwPollEvents();
  }

  // onExit
  glfwDestroyWindow(rawWindow);
  glfwTerminate();
}
