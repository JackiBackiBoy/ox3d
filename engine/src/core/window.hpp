#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core.hpp"
#include <string>

namespace ox {
  class OX_API Window {
    public:
      Window(const int& width, const int& height, const std::string& title);
      ~Window();

      Window(const Window&) = delete;
      Window &operator=(const Window&) = delete;

      inline bool shouldClose() { return glfwWindowShouldClose(m_RawWindow); }
      VkExtent2D getExtent();
      inline bool wasWindowResized() { return m_FramebufferResized; }
      inline void resetWindowResizedFlag() { m_FramebufferResized = false; }
      void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

    private:
      static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
      void createWindow();

      int m_Width;
      int m_Height;
      bool m_FramebufferResized = false;
      std::string m_Title;
      GLFWwindow* m_RawWindow = nullptr;
  };
}
