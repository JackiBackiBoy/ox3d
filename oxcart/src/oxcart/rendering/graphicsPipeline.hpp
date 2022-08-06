#pragma once

#include "oxcart/core/core.hpp"
#include "oxcart/rendering/graphicsDevice.hpp"
#include <string>
#include <vector>

namespace ox {
  struct OX_API PipelineConfigInfo {
    PipelineConfigInfo(const PipelineConfigInfo&) = delete;
    PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;
    PipelineConfigInfo() = default;

    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    std::vector<VkDynamicState> dynamicStateEnables;
    VkPipelineDynamicStateCreateInfo dynamicStateInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
  };

  class OX_API GraphicsPipeline {
    public:
      GraphicsPipeline(
          GraphicsDevice& device,
          const std::string& vertPath,
          const std::string& fragPath,
          const PipelineConfigInfo& configInfo);
      ~GraphicsPipeline();

      GraphicsPipeline(const GraphicsPipeline&) = delete;
      GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;

      void bind(VkCommandBuffer commandBuffer);
      static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

    private:
      void createGraphicsPipeline(
          const std::string& vertPath,
          const std::string& fragPath,
          const PipelineConfigInfo& configInfo);

      void createShaderModule(
          const std::vector<char>& code,
          VkShaderModule* shaderModule);

      GraphicsDevice& m_Device;
      VkPipeline m_GraphicsPipeline;
      VkShaderModule vertShaderModule;
      VkShaderModule fragShaderModule;

      static std::vector<char> readFile(const std::string& path);
  };
}
