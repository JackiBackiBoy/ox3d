#include "window.hpp"
#include <iostream>
#include <vector>
#include "oxcart/input/mouse.hpp"

using namespace ox;

Window::Window(const int& width, const int& height, const std::string& title)
  : m_Width(width), m_Height(height), m_Title(title) {
  createWindow();
}

Window::~Window() {
  glfwDestroyWindow(m_RawWindow);
  glfwTerminate();
}

void Window::createWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  m_RawWindow = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(m_RawWindow, this);
  glfwSetFramebufferSizeCallback(m_RawWindow, framebufferResizeCallback);
  glfwSetInputMode(m_RawWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(m_RawWindow, Mouse::onMouseMove);

}

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
  if (glfwCreateWindowSurface(instance, m_RawWindow, nullptr, surface) != VK_SUCCESS) {
    throw std::runtime_error("VULKAN ERROR: Failed to create window surface!");
  }
}

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
  auto engineWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
  engineWindow->m_FramebufferResized = true;
  engineWindow->m_Width = width;
  engineWindow->m_Height = height;
}

VkExtent2D Window::getExtent() {
  return { static_cast<uint32_t>(m_Width), static_cast<uint32_t>(m_Height) };
}
