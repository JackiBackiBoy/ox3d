#pragma once

#include "core/core.hpp"
#include "rendering/graphicsDevice.hpp"
#include "data/entity.hpp"
#include "rendering/renderer.hpp"
#include "window.hpp"

#include <memory>
#include <vector>

namespace ox {
  class OX_API Application {
    public:
      int WIDTH = 720;
      int HEIGHT = 720;

      Application();
      ~Application();

      // Delete copy constructors
      Application(const Application&) = delete;
      Application& operator=(const Application&) = delete;

      void run();

    private:
      void loadEntities();

      Window m_Window{WIDTH, HEIGHT, "Vulkan Renderer"};
      GraphicsDevice m_Device{m_Window};
      Renderer m_Renderer{m_Window, m_Device};
      std::vector<Entity> m_Entities;
  };
}
