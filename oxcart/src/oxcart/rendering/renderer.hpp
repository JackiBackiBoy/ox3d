#pragma once

#include "oxcart/core/core.hpp"
#include "oxcart/rendering/graphicsDevice.hpp"
#include "oxcart/rendering/swapChain.hpp"
#include "oxcart/core/window.hpp"

#include <memory>
#include <vector>
#include <cassert>

namespace ox {
  class OX_API Renderer {
    public:
      Renderer(Window& window, GraphicsDevice& device);
      ~Renderer();

      // Delete copy constructors
      Renderer(const Renderer&) = delete;
      Renderer& operator=(const Renderer&) = delete;

      inline VkRenderPass getSwapChainRenderPass() const { return m_SwapChain->getRenderPass(); }
      inline float getAspectRatio() const { return m_SwapChain->extentAspectRatio(); }
      inline bool isFrameInProgress() const { return m_IsFrameStarted; }
      VkCommandBuffer getCurrentCommandBuffer() const;
      int getFrameIndex() const;

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
      uint32_t m_CurrentImageIndex{0};
      int m_CurrentFrameIndex;
      bool m_IsFrameStarted{false};
  };
}
