#include "oxcart/rendering/systems/pointLightSystem.hpp"
#include "oxcart/components/transform.hpp"
#include "oxcart/components/light.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <stdexcept>
#include <array>
#include <iostream>

namespace ox {
  struct PointLightPushConstants {
    glm::vec4 position{};
    glm::vec4 color{};
    float radius;
  };

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
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(PointLightPushConstants);

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

  void PointLightSystem::onUpdate(FrameInfo& frameInfo, GlobalUBO& ubo)
  {
    int lightIndex = 0;

    for (auto& ent : frameInfo.entities) {
      if (!ent->hasComponent<Light>()) continue;

      auto lightComponent = ent->getComponent<Light>();
      ubo.pointLights[lightIndex].position = glm::vec4(ent->getComponent<Transform>()->position, 0.0f);
      ubo.pointLights[lightIndex].color = glm::vec4(lightComponent->color, lightComponent->intensity);
      lightIndex++;
    }

    ubo.numLights = lightIndex;
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

    for (auto& ent : frameInfo.entities) {
      if (!ent->hasComponent<Light>()) { continue; }

      auto lightComponent = ent->getComponent<Light>();

      PointLightPushConstants push{};
      push.position = glm::vec4(ent->getComponent<Transform>()->position, 0.0f);
      push.color = glm::vec4(lightComponent->color, lightComponent->intensity);
      push.radius = lightComponent->radius;

      vkCmdPushConstants(
        frameInfo.commandBuffer,
        m_PipelineLayout,
        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
        0,
        sizeof(PointLightPushConstants),
        &push
      );

      vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
    }
  }
}
