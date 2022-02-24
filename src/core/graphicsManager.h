#ifndef GRAPHICS_MANAGER_HEADER
#define GRAPHICS_MANAGER_HEADER

#include "window.h"
#include <vector>
#include <optional>

const std::vector<const char*> validationLayers = {
  "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
  VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

// If No-Debug (NDEBUG) is defined, then no validation layers will be created
#ifdef NDEBUG
  const bool enableValidationLayers = false;
#else
  const bool enableValidationLayers = true;
#endif

// Queue Family Indices struct
struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  inline bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

// Graphics Manager class
class GraphicsManager {
  public:
    GraphicsManager(Window* window);

    void loadVulkan();
    void destroyVulkan();
    void addShader(Shader* shader);

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

    Window* m_Window;
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
    VkPipelineLayout m_PipelineLayout;
    VkPipeline m_GraphicsPipeline;
};
#endif
