#pragma once

#include "core/core.hpp"
#include "rendering/graphicsDevice.hpp"
#include "rendering/swapChain.hpp"
#include "core/window.hpp"

#include <memory>
#include <vector>

namespace ox {
  class OX_API Renderer {
    public:
      Renderer(Window& window, GraphicsDevice& device);
      ~Renderer();

      // Delete copy constructors
      Renderer(const Renderer&) = delete;
      Renderer& operator=(const Renderer&) = delete;

      inline bool isFrameInProgress() const { return m_IsFrameStarted; }
      VkCommandBuffer getCurrentCommandBuffer() const { return m_CommandBuffers[m_CurrentImageIndex]; }

      VkCommandBuffer beginFrame();
      void endFrame();
      void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
      void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
    private:
      void createCommandBuffers();
      void freeCommandBuffers();
      void recreateSwapChain();

      Window& m_Window;
      GraphicsDevice& m_Device;
      std::unique_ptr<SwapChain> m_SwapChain;
      std::vector<VkCommandBuffer> m_CommandBuffers;
      uint32_t m_CurrentImageIndex;
      bool m_IsFrameStarted;
  };
}
