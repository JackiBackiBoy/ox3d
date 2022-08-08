#pragma once

#include "oxcart/components/camera.hpp"
#include <vulkan/vulkan.h>
#include <vector>

namespace ox {
  struct FrameInfo {
    int frameIndex;
    float frameTime;
    VkCommandBuffer commandBuffer;
    Camera& camera;
    std::vector<VkDescriptorSet>& descriptorSets;
  };
}
