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
  class OX_API PointLightSystem {
    public:
      PointLightSystem(GraphicsDevice& device, VkRenderPass renderPass,
          std::vector<VkDescriptorSetLayout>& setLayouts);
      ~PointLightSystem();

      // Delete copy constructors
      PointLightSystem(const PointLightSystem&) = delete;
      PointLightSystem& operator=(const PointLightSystem&) = delete;

      void onUpdate(FrameInfo& frameInfo, GlobalUBO& ubo);
      void render(FrameInfo& frameInfo);
    private:
      void createPipelineLayout(std::vector<VkDescriptorSetLayout>& setLayouts);
      void createPipeline(VkRenderPass renderPass);

      GraphicsDevice& m_Device;
      std::unique_ptr<GraphicsPipeline> m_Pipeline;
      VkPipelineLayout m_PipelineLayout;
  };
}
