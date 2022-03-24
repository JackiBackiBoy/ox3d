#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#ifdef NDEBUG
  const bool enableValidationLayers = false;
#else
  const bool enableValidationLayers = true;
#endif

#include "core.h"
#include <vector>
#include <optional>
#include "rendering/vertex.h"
#include "rendering/shader.h"
#include <GLFW/glfw3.h>
#include "rendering/texture2D.h"
#include "rendering/model.h"

namespace ox {
  struct OX_API QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    inline bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
  };

  struct OX_API SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
  };

  class OX_API GraphicsManager {
    public:
      GraphicsManager() {};

      void submitModel(const Model& model);

      void loadVulkan();
      void destroyVulkan();
      void addShader(Shader* shader);
      void renderFrame();
      static void onResize(GLFWwindow* window, int width, int height);
      void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                        VkBuffer& buffer, VkDeviceMemory& bufferMemory);
      void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
                       VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
                       VkImage& image, VkDeviceMemory& imageMemory);
      void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout,
                                 VkImageLayout newLayout);
      void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
      void submitTexture(Texture2D* texture);

      // Getters
      inline VkDevice getLogicalDevice() { return m_LogicalDevice; }
      inline VkExtent2D getSwapChainExtent() { return m_SwapChainExtent; }

      std::vector<Vertex> m_Vertices;
      std::vector<uint32_t> m_Indices;

    private:
      bool checkValidationLayerSupport();
      void createVkInstance();
      void setupDebugMessenger();
      void createSurface();
      VkResult createDebugMessenger(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                    const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
      static void destroyDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                 const VkAllocationCallbacks* pAllocator);
      void populateDebugMessenger(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

      std::vector<const char*> getRequiredExtensions();
      static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                          VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                          const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                          void* pUserData);
      void pickPhysicalDevice();
      bool isDeviceSuitable(VkPhysicalDevice device);
      QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
      void createLogicalDevice();
      bool checkDeviceExtensionSupport(VkPhysicalDevice device);
      SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
      VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
      VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
      VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
      void createSwapChain();
      void createImageViews();
      void createRenderPass();
      void createGraphicsPipeline();
      VkShaderModule createShaderModule(const std::vector<char>& code);
      void createFramebuffers();
      void createCommandPool();
      void createCommandBuffers();
      void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
      void createSyncObjects();
      void recreateSwapChain();
      void cleanupSwapChain();
      void createVertexBuffer();
      void createIndexBuffer();
      void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
      uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
      void createDescriptorSetLayout();
      void createUniformBuffers();
      void updateUniformBuffer(uint32_t currentImage);
      void createDescriptorPool();
      void createDescriptorSets();
      void createTextureImages();
      VkCommandBuffer beginSingleTimeCommands();
      void endSingleTimeCommands(VkCommandBuffer commandBuffer);
      void createTextureImageView();
      VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
      void createTextureSampler();
      void createDepthResources();
      VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling,
                               VkFormatFeatureFlags features);
      VkFormat findDepthFormat();
      bool hasStencilComponent(VkFormat format);

      VkInstance m_Instance;
      VkDebugUtilsMessengerEXT m_DebugMessenger;
      VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
      VkDevice m_LogicalDevice;
      VkQueue m_GraphicsQueue;
      VkSurfaceKHR m_Surface;
      VkQueue m_PresentQueue;
      VkSwapchainKHR m_SwapChain;
      std::vector<VkImage> m_SwapChainImages;
      VkFormat m_SwapChainImageFormat;
      VkExtent2D m_SwapChainExtent;
      std::vector<VkImageView> m_SwapChainImageViews;
      std::vector<Shader*> m_Shaders;
      VkRenderPass m_RenderPass;
      VkDescriptorSetLayout m_DescriptorSetLayout;
      VkPipelineLayout m_PipelineLayout;
      VkPipeline m_GraphicsPipeline;
      std::vector<VkFramebuffer> m_SwapChainFramebuffers;
      VkCommandPool m_CommandPool;
      std::vector<VkCommandBuffer> m_CommandBuffers;
      std::vector<VkSemaphore> m_ImageAvailableSemaphores;
      std::vector<VkSemaphore> m_RenderFinishedSemaphores;
      std::vector<VkFence> m_InFlightFences;
      bool m_FramebufferResized = false;
      uint32_t m_CurrentFrame = 0;
      VkBuffer m_VertexBuffer;
      VkDeviceMemory m_VertexBufferMemory;
      VkBuffer m_IndexBuffer;
      VkDeviceMemory m_IndexBufferMemory;
      std::vector<VkBuffer> m_UniformBuffers;
      std::vector<VkDeviceMemory> m_UniformBuffersMemory;
      VkDescriptorPool m_DescriptorPool;
      std::vector<VkDescriptorSet> m_DescriptorSets;
      VkImageView m_TextureImageView;
      VkSampler m_TextureSampler;
      VkImage m_DepthImage;
      VkDeviceMemory m_DepthImageMemory;
      VkImageView m_DepthImageView;

      std::vector<Texture2D*> m_TextureSubmits;

      // Constants
      static constexpr int MAX_FRAMES_IN_FLIGHT = 2;
      static const std::vector<const char*> deviceExtensions; 
      static const std::vector<const char*> validationLayers;
  };
}
