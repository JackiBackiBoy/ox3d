#pragma once

#include "core/core.hpp"
#include "rendering/graphicsDevice.hpp"
#include "rendering/model.hpp"
#include "rendering/graphicsPipeline.hpp"
#include "rendering/swapChain.hpp"
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
      void loadModels();
      void createPipelineLayout();
      void createPipeline();
      void createCommandBuffers();
      void freeCommandBuffers();
      void drawFrame();
      void recreateSwapChain();
      void recordCommandBuffer(int imageIndex);

      Window m_Window{WIDTH, HEIGHT, "Vulkan Renderer"};
      GraphicsDevice m_Device{m_Window};
      std::unique_ptr<SwapChain> m_SwapChain;
      std::unique_ptr<GraphicsPipeline> m_Pipeline;
      VkPipelineLayout m_PipelineLayout;
      std::vector<VkCommandBuffer> m_CommandBuffers;
      std::unique_ptr<Model> m_Model;
  };
}
