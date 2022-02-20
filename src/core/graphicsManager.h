#ifndef GRAPHICS_MANAGER_HEADER
#define GRAPHICS_MANAGER_HEADER

#include "vulkan/vulkan.h"
#include <GLFW/glfw3.h>
#include "window.h"

class GraphicsManager {
  public:
    GraphicsManager() {};

    void loadVulkan();
    void destroyVulkan();

  private:
    Window* m_Window;
    VkInstance m_Instance;
};
#endif
