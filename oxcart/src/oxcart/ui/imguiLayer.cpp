#include "oxcart/ui/imguiLayer.hpp"
#include "oxcart/ui/imgui.h"
#include "oxcart/ui/imgui_impl_glfw.h"
#include "oxcart/ui/imgui_impl_vulkan.h"
#include "oxcart/ui/imguiLayer.hpp"

// std
#include <stdexcept>

namespace ox {
  void ImGuiLayer::initialize(GraphicsDevice& device, Renderer& renderer) {
    // ImGui descriptor pool
    VkDescriptorPoolSize pool_sizes[] =
    {
      { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
      { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
      { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
      { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
      { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
      { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
      { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
      { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
      { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
      { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
      { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
    };

    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
    pool_info.poolSizeCount = static_cast<uint32_t>(IM_ARRAYSIZE(pool_sizes));
    pool_info.pPoolSizes = pool_sizes;
    const auto& swapChainSupport = device.getSwapChainSupport();

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 &&
			imageCount > swapChainSupport.capabilities.maxImageCount) {
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

    if(vkCreateDescriptorPool(device.device(), &pool_info, nullptr, &m_Pool) != VK_SUCCESS) {
      throw std::runtime_error("VULKAN ERROR: ImGui descriptor pool could not be created!");
    }

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup ImGui style
    ImGui::StyleColorsDark();

    // Setup platform/renderer bindings
    ImGui_ImplGlfw_InitForVulkan(device.getWindow().getRawWindow(), true);
    ImGui_ImplVulkan_InitInfo initInfo = {};
    initInfo.Instance = device.getInstance();
    initInfo.PhysicalDevice = device.getPhysicalDevice();
    initInfo.Device = device.device();
    initInfo.Queue = device.graphicsQueue();
    initInfo.QueueFamily = device.findPhysicalQueueFamilies().graphicsFamily;
    initInfo.DescriptorPool = m_Pool;
    initInfo.MinImageCount = imageCount;
    initInfo.ImageCount = imageCount;
    initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    ImGui_ImplVulkan_Init(&initInfo, renderer.getSwapChainRenderPass());

    VkCommandBuffer command_buffer = device.beginSingleTimeCommands();
    ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
    device.endSingleTimeCommands(command_buffer);
    ImGui_ImplVulkan_DestroyFontUploadObjects(); // clear font textures from CPU
  }

  void ImGuiLayer::render(FrameInfo& frameInfo) {
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Object editor pane
    if (ImGui::Begin("Object Editor")) {
      static float rotation = 0.0f;
      ImGui::SliderFloat("Rotation", &rotation, 0, 360);
    }
    ImGui::End();

    ImGui::Render();

    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), frameInfo.commandBuffer);
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
  }

  bool ImGuiLayer::isUsingMouse() {
    return ImGui::GetIO().WantCaptureMouse;
  }

  bool ImGuiLayer::isUsingKeyboard() {
    return ImGui::GetIO().WantCaptureKeyboard;
  }

  VkDescriptorPool ImGuiLayer::m_Pool;
}
