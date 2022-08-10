#pragma once

#include "oxcart/core/core.hpp"
#include "oxcart/components/camera.hpp"
#include <vulkan/vulkan.h>
#include <vector>

namespace ox {
  struct OX_API FrameInfo {
    int frameIndex;
    float frameTime;
    VkCommandBuffer commandBuffer;
    Camera& camera;
    std::vector<VkDescriptorSet>& descriptorSets;
  };
}
