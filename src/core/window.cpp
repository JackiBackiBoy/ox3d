#include "window.h"
#include <iostream>
#include <vector>
#include "graphicsManager.h"

void Window::createWindow() {
  glfwInit(); // initialize glfw

  // set GLFW options
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  m_RawWindow = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
  
  // Supported extensions
  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

  // Store all extensions in extensions vector
  std::vector<VkExtensionProperties> extensions(extensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

  // List all supported extensions
  std::cout << "Extensions supported: " << extensionCount << "\n";
  for (auto& extension : extensions) {
    std::cout << "\t" << extension.extensionName << "\n";
  }
}

void Window::run() {
  createWindow();

  // Rendering loop
  while (!glfwWindowShouldClose(m_RawWindow)) {
    glfwPollEvents();
  }

  // onExit
  glfwDestroyWindow(m_RawWindow);
  glfwTerminate();
}
