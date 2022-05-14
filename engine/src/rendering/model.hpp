#pragma once

#include "core/core.hpp"
#include "graphicsDevice.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace ox {
  class OX_API Model {
    public:
      struct OX_API Vertex {
        glm::vec2 position;
        glm::vec3 color;

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
      };

      Model(GraphicsDevice& device, const std::vector<Vertex>& vertices);
      ~Model();

      // Delete copy constructors
      Model(const Model&) = delete;
      Model &operator=(const Model&) = delete;

      void bind(VkCommandBuffer commandBuffer);
      void draw(VkCommandBuffer commandBuffer);

    private:
      void createVertexBuffers(const std::vector<Vertex>& vertics);

      GraphicsDevice& m_Device;
      VkBuffer m_VertexBuffer;
      VkDeviceMemory m_VertexBufferMemory;
      uint32_t m_VertexCount;
  };
}
