#pragma once

#include "oxcart/core/core.hpp"
#include "oxcart/rendering/graphicsDevice.hpp"

// vulkan headers
#include <vulkan/vulkan.h>

// std lib headers
#include <memory>
#include <string>
#include <vector>

namespace ox {
  class OX_API SwapChain {
   public:
    static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    SwapChain(GraphicsDevice &deviceRef, VkExtent2D windowExtent);
    SwapChain(GraphicsDevice &deviceRef, VkExtent2D windowExtent, std::shared_ptr<SwapChain> previous);
    ~SwapChain();

    SwapChain(const SwapChain &) = delete;
    SwapChain& operator=(const SwapChain &) = delete;

    VkFramebuffer getFrameBuffer(int index) { return m_SwapChainFramebuffers[index]; }
    VkRenderPass getRenderPass() { return m_RenderPass; }
    VkImageView getImageView(int index) { return m_SwapChainImageViews[index]; }
    size_t imageCount() { return m_SwapChainImages.size(); }
    VkFormat getSwapChainImageFormat() { return m_SwapChainImageFormat; }
    VkExtent2D getSwapChainExtent() { return m_SwapChainExtent; }
    uint32_t width() { return m_SwapChainExtent.width; }
    uint32_t height() { return m_SwapChainExtent.height; }

    float extentAspectRatio() {
      return static_cast<float>(m_SwapChainExtent.width) / static_cast<float>(m_SwapChainExtent.height);
    }
    VkFormat findDepthFormat();

    VkResult acquireNextImage(uint32_t *imageIndex);
    VkResult submitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *imageIndex);

    bool compareSwapFormats(const SwapChain& swapChain) const;
    static VkSampler m_TextureSampler;

   private:
    void init();
    void createSwapChain();
    void createImageViews();
    void createTextureSampler();
    void createDepthResources();
    void createRenderPass();
    void createFramebuffers();
    void createSyncObjects();

    // Helper functions
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(
        const std::vector<VkPresentModeKHR> &availablePresentModes, const bool& vSync);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    VkFormat m_SwapChainImageFormat;
    VkFormat m_SwapChainDepthFormat;
    VkExtent2D m_SwapChainExtent;

    std::vector<VkFramebuffer> m_SwapChainFramebuffers;
    VkRenderPass m_RenderPass;

    std::vector<VkImage> m_DepthImages;
    std::vector<VkDeviceMemory> m_DepthImageMemorys;
    std::vector<VkImageView> m_DepthImageViews;
    std::vector<VkImage> m_SwapChainImages;
    std::vector<VkImageView> m_SwapChainImageViews;

    GraphicsDevice &m_Device;
    VkExtent2D m_WindowExtent;

    VkSwapchainKHR m_SwapChain;
    std::shared_ptr<SwapChain> m_OldSwapChain;

    std::vector<VkSemaphore> m_ImageAvailableSemaphores;
    std::vector<VkSemaphore> m_RenderFinishedSemaphores;
    std::vector<VkFence> m_InFlightFences;
    std::vector<VkFence> m_ImagesInFlight;
    size_t m_CurrentFrame = 0;
  };
}
