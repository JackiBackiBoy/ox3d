#pragma once

#include "oxcart/components/camera.hpp"
#include <vulkan/vulkan.h>

namespace ox {
  struct FrameInfo {
    int frameIndex;
    float frameTime;
    VkCommandBuffer commandBuffer;
    Camera& camera;
    VkDescriptorSet globalDescriptorSet;
  };
}
