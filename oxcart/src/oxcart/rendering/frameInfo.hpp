#pragma once

#include "oxcart/core/core.hpp"
#include "oxcart/components/camera.hpp"
#include <vulkan/vulkan.h>
#include <vector>

namespace ox {

#define MAX_LIGHTS 16

  struct PointLight {
    glm::vec4 position{}; // ignore w
    glm::vec4 color{}; // w is intensity
  };
  struct OX_API GlobalUBO {
    glm::mat4 projection{ 1.0f };
    glm::mat4 view{ 1.0f };
    glm::mat4 inverseView{ 1.0f };
    glm::vec4 ambientLightColor{ 1.0f, 1.0f, 1.0f, 0.02f }; // w is intensity
    PointLight pointLights[MAX_LIGHTS];
    int numLights{};
  };

  struct OX_API FrameInfo {
    int frameIndex;
    float frameTime;
    VkCommandBuffer commandBuffer;
    Camera& camera;
    std::vector<VkDescriptorSet>& descriptorSets;
    std::vector<Entity*>& entities;
  };
}
