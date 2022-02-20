#ifndef WINDOW_HEADER
#define WINDOW_HEADER
#define GLFW_INCLUDE_VULKAN

class GraphicsManager;

#include <stdint.h>
#include <string>
#include <GLFW/glfw3.h>

class Window {
  public:
    Window(const uint32_t& width, const uint32_t& height, const std::string& title);
    ~Window();

    void run();

    inline std::string getTitle() { return m_Title; }

  private:
    void createWindow();

    uint32_t m_Width;
    uint32_t m_Height;
    std::string m_Title;
    GLFWwindow* m_RawWindow;
    GraphicsManager* m_GraphicsManager;
};
#endif
