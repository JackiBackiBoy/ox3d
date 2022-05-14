#include "application.hpp"
#include <stdexcept>
#include <array>
#include <iostream>

namespace ox {
  Application::Application() {
    loadModels();
    createPipelineLayout();
    recreateSwapChain();
    createCommandBuffers();
  }

  Application::~Application() {
    vkDestroyPipelineLayout(m_Device.device(), m_PipelineLayout, nullptr);
  }

  void Application::run() {
    while (!m_Window.shouldClose()) {
      glfwPollEvents();
      drawFrame();
    }

    vkDeviceWaitIdle(m_Device.device());
  }

  float sin45 = sin(glm::radians(45.0f));
  std::vector<Model::Vertex> sierpinski(
      const int& iterations,
      const float& sideLen,
      const float& originX,
      const float& originY) {
    float height = sideLen * sin45;

    if (iterations == 0) {
      // Create 3 new equilateral triangles
      // Top triangle
      glm::vec2 a1 = { originX, originY - height / 2.0f }; // top
      glm::vec2 b1 = { originX + sideLen / 4.0f,  originY }; // bottom right
      glm::vec2 c1 = { originX - sideLen / 4.0f,  originY }; // bottom left

      // Bottom right triangle
      glm::vec2 a2 = { originX + sideLen / 4.0f, originY }; // top
      glm::vec2 b2 = { originX + sideLen / 2.0f, originY + height / 2.0f }; // bottom right
      glm::vec2 c2 = { originX, originY + height / 2.0f }; // bottom left

      // Bottom left triangle
      glm::vec2 a3 = { originX - sideLen / 4.0f, originY }; // top
      glm::vec2 b3 = { originX, originY + height / 2.0f }; // bottom right
      glm::vec2 c3 = { originX - sideLen / 2.0f, originY + height / 2.0f }; // bottom left

      std::vector<Model::Vertex> newVertices;
      newVertices.push_back({ a1, { 1.0f, 0.0f, 0.0f } });
      newVertices.push_back({ b1, { 1.0f, 0.0f, 0.0f } });
      newVertices.push_back({ c1, { 1.0f, 0.0f, 0.0f } });

      newVertices.push_back({ a2, { 1.0f, 0.0f, 0.0f } });
      newVertices.push_back({ b2, { 1.0f, 0.0f, 0.0f } });
      newVertices.push_back({ c2, { 1.0f, 0.0f, 0.0f } });

      newVertices.push_back({ a3, { 1.0f, 0.0f, 0.0f } });
      newVertices.push_back({ b3, { 1.0f, 0.0f, 0.0f } });
      newVertices.push_back({ c3, { 1.0f, 0.0f, 0.0f } });

      return newVertices;
    }
    else {
      auto topVertices = sierpinski(iterations - 1, sideLen / 2.0f, originX, originY - height / 4.0f); // top
      auto bottomRightVertices = sierpinski(iterations - 1, sideLen / 2.0f, originX + sideLen / 4.0f, originY + height / 4.0f); // bottom right
      auto bottomLeftVertices = sierpinski(iterations - 1, sideLen / 2.0f, originX - sideLen / 4.0f, originY + height / 4.0f); // bottom left


      std::vector<Model::Vertex> newVertices;
      for (auto t : topVertices) {
        newVertices.push_back(t);
      }

      for (auto br : bottomRightVertices) {
        newVertices.push_back(br);
      }
      for (auto bl : bottomLeftVertices) {
        newVertices.push_back(bl);
      }

      return newVertices;
    }
  }

  void Application::loadModels() {
    std::vector<Model::Vertex> vertices {
      //{ {  0.0f, -(sideLen * sin45) / 2.0f } },
      //{ {  sideLen / 2.0f,  (sideLen * sin45) / 2.0f } },
      //{ { -sideLen / 2.0f,  (sideLen * sin45) / 2.0f } }
    };

    vertices = sierpinski(4, 1.0f, 0.0f, 0.0f);
    std::cout << vertices.size() << std::endl;

    m_Model = std::make_unique<Model>(m_Device, vertices);
  }

  void Application::createPipelineLayout() {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(
          m_Device.device(),
          &pipelineLayoutInfo,
          nullptr,
          &m_PipelineLayout) != VK_SUCCESS) {
      throw std::runtime_error("VULKAN ERROR: Failed to create pipeline layout!");
    }
  }

  void Application::createPipeline() {
    assert(m_SwapChain != nullptr && "VULKAN ASSERTION FAILED: Cannot create pipeline before swap chain!");
    assert(m_PipelineLayout != nullptr && "VULKAN ASSERTION FAILED: Cannot create pipeline before pipeline layout!");

    PipelineConfigInfo pipelineConfig{};
    GraphicsPipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = m_SwapChain->getRenderPass();
    pipelineConfig.pipelineLayout = m_PipelineLayout;
    m_Pipeline = std::make_unique<GraphicsPipeline>(
        m_Device,
        "assets/shaders/lightingShader.vert.spv",
        "assets/shaders/lightingShader.frag.spv",
        pipelineConfig);
  }

  void Application::recreateSwapChain() {
    auto extent = m_Window.getExtent();

    while (extent.width == 0 || extent.height == 0) {
      extent = m_Window.getExtent();
      glfwWaitEvents();
    }

    vkDeviceWaitIdle(m_Device.device());

    if (m_SwapChain == nullptr) {
      m_SwapChain = std::make_unique<SwapChain>(m_Device, extent);
    }
    else {
      m_SwapChain = std::make_unique<SwapChain>(m_Device, extent, std::move(m_SwapChain));

      if (m_SwapChain->imageCount() != m_CommandBuffers.size()) {
        freeCommandBuffers();
        createCommandBuffers();
      }
    }

    // TODO: if render pass is compatible, then do nothing
    createPipeline();
  }

  void Application::createCommandBuffers() {
    m_CommandBuffers.resize(m_SwapChain->imageCount());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_Device.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());

    if (vkAllocateCommandBuffers(m_Device.device(), &allocInfo, m_CommandBuffers.data()) !=
        VK_SUCCESS) {
      throw std::runtime_error("VULKAN ERROR: Failed to allocate command buffers!");
    }
  }

  void Application::freeCommandBuffers() {
    vkFreeCommandBuffers(
        m_Device.device(),
        m_Device.getCommandPool(),
        static_cast<uint32_t>(m_CommandBuffers.size()),
        m_CommandBuffers.data());
    m_CommandBuffers.clear();
  }

  void Application::recordCommandBuffer(int imageIndex) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(m_CommandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
      throw std::runtime_error("VULKAN ERROR: Failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = m_SwapChain->getRenderPass();
    renderPassInfo.framebuffer = m_SwapChain->getFrameBuffer(imageIndex);
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = m_SwapChain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(m_CommandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(m_SwapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(m_SwapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, m_SwapChain->getSwapChainExtent()};
    vkCmdSetViewport(m_CommandBuffers[imageIndex], 0, 1, &viewport);
    vkCmdSetScissor(m_CommandBuffers[imageIndex], 0, 1, &scissor);

    m_Pipeline->bind(m_CommandBuffers[imageIndex]);
    m_Model->bind(m_CommandBuffers[imageIndex]);
    m_Model->draw(m_CommandBuffers[imageIndex]);

    vkCmdEndRenderPass(m_CommandBuffers[imageIndex]);

    if (vkEndCommandBuffer(m_CommandBuffers[imageIndex]) != VK_SUCCESS) {
      throw std::runtime_error("VULKAN ERROR: Failed to record command buffer!");
    }
  }

  void Application::drawFrame() {
    uint32_t imageIndex;
    auto result = m_SwapChain->acquireNextImage(&imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      recreateSwapChain();
      return;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      throw std::runtime_error("VULKAN ERROR: Failed to acquire swap chain image!");
    }

    recordCommandBuffer(imageIndex);
    result = m_SwapChain->submitCommandBuffers(&m_CommandBuffers[imageIndex], &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
        m_Window.wasWindowResized()) {
      m_Window.resetWindowResizedFlag();
      recreateSwapChain();
      return;
    }

    if (result != VK_SUCCESS) {
      throw std::runtime_error("VULKAN ERROR: Failed to present swap chain image!");
    }
  }
}
