#ifndef WINDOW_HEADER
#define WINDOW_HEADER
#define GLFW_INCLUDE_VULKAN

#include <stdint.h>
#include <string>
#include <GLFW/glfw3.h>

class Window {
  public:
    Window(const uint32_t& width, const uint32_t& height, const std::string& title)
      : m_Width(width), m_Height(height), m_Title(title) {}

    void run();

  private:
    void createWindow();

    uint32_t m_Width;
    uint32_t m_Height;
    std::string m_Title;
    GLFWwindow* rawWindow;
};
#endif
