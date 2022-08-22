#include "oxcart/rendering/systems/renderSystem.hpp"
#include "oxcart/components/transform.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <stdexcept>
#include <array>
#include <iostream>

namespace ox {
  RenderSystem::RenderSystem(GraphicsDevice& device, VkRenderPass renderPass,
      std::vector<VkDescriptorSetLayout>& setLayouts)
      : m_Device{device} {
    createPipelineLayout(setLayouts);
    createPipeline(renderPass);
  }

  RenderSystem::~RenderSystem() {
    vkDestroyPipelineLayout(m_Device.device(), m_PipelineLayout, nullptr);
  }

  void RenderSystem::createPipelineLayout(std::vector<VkDescriptorSetLayout>& setLayouts) {
    // Push constants
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(setLayouts.size());
    pipelineLayoutInfo.pSetLayouts = setLayouts.data();
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

  void RenderSystem::renderEntities(FrameInfo& frameInfo, std::vector<Entity*>& entities) {
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

    for (auto& ent: entities) {
      if (ent->model != nullptr) {

        SimplePushConstantData push{};
        push.modelMatrix = ent->getComponent<Transform>()->mat4();
        push.normalMatrix = ent->getComponent<Transform>()->normalMatrix();

        vkCmdPushConstants(
            frameInfo.commandBuffer,
            m_PipelineLayout,
            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
            0,
            sizeof(SimplePushConstantData),
            &push);

        ent->model->bind(frameInfo.commandBuffer);
        ent->model->draw(frameInfo.commandBuffer, m_PipelineLayout);
      }
    }
  }
}
