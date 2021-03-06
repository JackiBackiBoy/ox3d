#pragma once

#include "core/core.hpp"
#include "rendering/graphicsDevice.hpp"
#include "data/entity.hpp"
#include "rendering/graphicsPipeline.hpp"

#include <memory>
#include <vector>

namespace ox {
  struct OX_API SimplePushConstantData {
    glm::mat2 transform{1.0f};
    glm::vec2 offset;
    alignas(16) glm::vec3 color;
  };
  
  class OX_API RenderSystem {
    public:
      RenderSystem(GraphicsDevice& device, VkRenderPass renderPass);
      ~RenderSystem();

      // Delete copy constructors
      RenderSystem(const RenderSystem&) = delete;
      RenderSystem& operator=(const RenderSystem&) = delete;

      void renderEntities(VkCommandBuffer commandBuffer, std::vector<Entity>& entities);
    private:
      void createPipelineLayout();
      void createPipeline(VkRenderPass renderPass);

      GraphicsDevice& m_Device;
      std::unique_ptr<GraphicsPipeline> m_Pipeline;
      VkPipelineLayout m_PipelineLayout;
  };
}
