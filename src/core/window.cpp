#include "window.h"
#include <iostream>
#include <vector>

void Window::createWindow() {
  glfwInit(); // initialize glfw

  // set GLFW options
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  rawWindow = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
  
  // Supported extensions
  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

  // Store all extensions in extensions vector
  std::vector<VkExtensionProperties> extensions(extensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

  std::cout << "Extensions supported: " << extensionCount << "\n";
  for (auto& extension : extensions) {
    std::cout << "\t" << extension.extensionName << "\n";
  }
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
