#include "oxcart/rendering/systems/pointLightSystem.hpp"
#include "oxcart/components/transform.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <stdexcept>
#include <array>
#include <iostream>

namespace ox {
  PointLightSystem::PointLightSystem(GraphicsDevice& device, VkRenderPass renderPass,
      std::vector<VkDescriptorSetLayout>& setLayouts)
      : m_Device{device} {
    createPipelineLayout(setLayouts);
    createPipeline(renderPass);
  }

  PointLightSystem::~PointLightSystem() {
    vkDestroyPipelineLayout(m_Device.device(), m_PipelineLayout, nullptr);
  }

  void PointLightSystem::createPipelineLayout(std::vector<VkDescriptorSetLayout>& setLayouts) {
    // Push constants
    //VkPushConstantRange pushConstantRange{};
    //pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    //pushConstantRange.offset = 0;
    //pushConstantRange.size = sizeof(SimplePushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(setLayouts.size());
    pipelineLayoutInfo.pSetLayouts = setLayouts.data();
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

  void PointLightSystem::createPipeline(VkRenderPass renderPass) {
    assert(m_PipelineLayout != nullptr && "VULKAN ASSERTION FAILED: Cannot create pipeline before pipeline layout!");

    PipelineConfigInfo pipelineConfig{};
    GraphicsPipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.attributeDescriptions.clear();
    pipelineConfig.bindingDescriptions.clear();
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = m_PipelineLayout;
    m_Pipeline = std::make_unique<GraphicsPipeline>(
        m_Device,
        "assets/shaders/pointLight.vert.spv",
        "assets/shaders/pointLight.frag.spv",
        pipelineConfig);
  }

  void PointLightSystem::render(FrameInfo& frameInfo) {
    m_Pipeline->bind(frameInfo.commandBuffer);

    // Binding of descriptor sets (ubo, texture samplers)
    //===================================================
    vkCmdBindDescriptorSets(
        frameInfo.commandBuffer,
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        m_PipelineLayout,
        0,
        1,
        &frameInfo.descriptorSets[0],
        0,
        nullptr);

    vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
  }
}
