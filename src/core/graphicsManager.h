#ifndef GRAPHICS_MANAGER_HEADER
#define GRAPHICS_MANAGER_HEADER

#include "vulkan/vulkan.h"
#include "window.h"

class GraphicsManager {
  public:
    GraphicsManager() {};

    void loadVulkan();

  private:
    Window* m_Window;
    VkInstance m_Instance;
};
#endif
