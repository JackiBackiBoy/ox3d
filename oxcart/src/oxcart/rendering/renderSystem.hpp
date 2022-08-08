#pragma once

#include "oxcart/core/core.hpp"
#include "oxcart/components/camera.hpp"
#include "oxcart/rendering/graphicsDevice.hpp"
#include "oxcart/data/entity.hpp"
#include "oxcart/rendering/graphicsPipeline.hpp"
#include "oxcart/rendering/frameInfo.hpp"

#include <memory>
#include <vector>

namespace ox {
  struct OX_API SimplePushConstantData {
    glm::mat4 modelMatrix{1.0f};
    glm::mat4 normalMatrix{1.0f};
  };
  
  class OX_API RenderSystem {
    public:
      RenderSystem(GraphicsDevice& device, VkRenderPass renderPass,
          std::vector<VkDescriptorSetLayout>& setLayouts);
      ~RenderSystem();

      // Delete copy constructors
      RenderSystem(const RenderSystem&) = delete;
      RenderSystem& operator=(const RenderSystem&) = delete;

      void renderEntities(FrameInfo& frameInfo, std::vector<Entity>& entities);
    private:
      void createPipelineLayout(std::vector<VkDescriptorSetLayout>& setLayouts);
      void createPipeline(VkRenderPass renderPass);

      GraphicsDevice& m_Device;
      std::unique_ptr<GraphicsPipeline> m_Pipeline;
      VkPipelineLayout m_PipelineLayout;
  };
}
