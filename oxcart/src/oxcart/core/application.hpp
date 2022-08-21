#pragma once

#include "oxcart/core/core.hpp"
#include "oxcart/core/window.hpp"
#include "oxcart/data/entity.hpp"
#include "oxcart/rendering/descriptors.hpp"
#include "oxcart/rendering/graphicsDevice.hpp"
#include "oxcart/rendering/renderer.hpp"
#include "oxcart/rendering/systems/renderSystem.hpp"
#include "oxcart/rendering/systems/pointLightSystem.hpp"

// std
#include <memory>
#include <vector>
#include <functional>

namespace ox {
  class OX_API Application {
    public:
      int WIDTH = 1920;
      int HEIGHT = 1080;

      Application();
      ~Application();

      // Delete copy constructors
      Application(const Application&) = delete;
      Application& operator=(const Application&) = delete;

      virtual void onStart();
      virtual void onUpdate(const float& dt);
      virtual void onRender(const float& dt);

      void run();

    protected:
      Window m_Window{WIDTH, HEIGHT, "Oxcart | Vulkan Renderer"};
      GraphicsDevice m_Device{m_Window};
      Renderer m_Renderer{m_Window, m_Device};

      std::unique_ptr<DescriptorPool> globalPool{};
      std::vector<Entity*> m_Entities;

    private:
      VkDescriptorPool imguiPool = VK_NULL_HANDLE;
      glm::vec2 lastMousePos;
      float lastAspectRatio = 0;
      std::vector<VkDescriptorSet> globalDescriptorSets;
      std::vector<std::unique_ptr<Buffer>> uboBuffers;
      std::unique_ptr<RenderSystem> renderSystem;
      std::unique_ptr<PointLightSystem> pointLightSystem;
  };
}
