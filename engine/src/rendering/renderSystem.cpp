#include "renderSystem.hpp"
#include <stdexcept>
#include <array>
#include <iostream>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace ox {
  RenderSystem::RenderSystem(GraphicsDevice& device, VkRenderPass renderPass)
      : m_Device{device} {
    createPipelineLayout();
    createPipeline(renderPass);
  }

  RenderSystem::~RenderSystem() {
    vkDestroyPipelineLayout(m_Device.device(), m_PipelineLayout, nullptr);
  }

  void RenderSystem::createPipelineLayout() {
    // Push constants
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

    if (vkCreatePipelineLayout(
          m_Device.device(),
          &pipelineLayoutInfo,
          nullptr,
          &m_PipelineLayout) != VK_SUCCESS) {
      throw std::runtime_error("VULKAN ERROR: Failed to create pipeline layout!");
    }
  }

  void RenderSystem::createPipeline(VkRenderPass renderPass) {
    assert(m_PipelineLayout != nullptr && "VULKAN ASSERTION FAILED: Cannot create pipeline before pipeline layout!");

    PipelineConfigInfo pipelineConfig{};
    GraphicsPipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = m_PipelineLayout;
    m_Pipeline = std::make_unique<GraphicsPipeline>(
        m_Device,
        "assets/shaders/lightingShader.vert.spv",
        "assets/shaders/lightingShader.frag.spv",
        pipelineConfig);
  }

  void RenderSystem::renderEntities(VkCommandBuffer commandBuffer, std::vector<Entity>& entities) {
    m_Pipeline->bind(commandBuffer);

    for (auto& obj: entities) {
      obj.transform.rotation.y = glm::mod(obj.transform.rotation.y + 0.01f, glm::two_pi<float>());

      SimplePushConstantData push{};
      push.transform = obj.transform.mat4();
      push.color = obj.color;

      vkCmdPushConstants(
          commandBuffer,
          m_PipelineLayout,
          VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
          0,
          sizeof(SimplePushConstantData),
          &push);

      obj.model->bind(commandBuffer);
      obj.model->draw(commandBuffer);
    }
  }
}
