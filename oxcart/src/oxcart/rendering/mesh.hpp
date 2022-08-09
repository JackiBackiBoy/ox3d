#pragma once

#include "oxcart/core/core.hpp"
#include "oxcart/rendering/graphicsDevice.hpp"
#include <vulkan/vulkan.h>
#include <cstdint>
#include <vector>

namespace ox {
  class OX_API Mesh {
    public:
      Mesh();
      ~Mesh();

      uint32_t indices;
      uint32_t materialIndex;
      uint32_t baseVertex;
      uint32_t baseIndex;
      VkDescriptorSet m_DescriptorSet;

    private:
  };
}
