#include "model.hpp"
#include <cassert>
#include <cstring>

namespace ox {
  Model::Model(GraphicsDevice& device, const std::vector<Vertex>& vertices)
    : m_Device(device) {
    createVertexBuffers(vertices);
  }

  Model::~Model() {
    vkDestroyBuffer(m_Device.device(), m_VertexBuffer, nullptr);
    vkFreeMemory(m_Device.device(), m_VertexBufferMemory, nullptr);
  }

  void Model::createVertexBuffers(const std::vector<Vertex>& vertices) {
    m_VertexCount = static_cast<uint32_t>(vertices.size());
    assert(m_VertexCount >= 3 && "VULKAN ASSERTION FAILED: Vertex count must be >= 3");

    VkDeviceSize bufferSize = sizeof(vertices[0]) * m_VertexCount;
    m_Device.createBuffer(
        bufferSize,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        m_VertexBuffer,
        m_VertexBufferMemory);

    void* data;
    vkMapMemory(m_Device.device(), m_VertexBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
    vkUnmapMemory(m_Device.device(), m_VertexBufferMemory);
  }

  void Model::bind(VkCommandBuffer commandBuffer) {
    VkBuffer buffers[] = {m_VertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
  }

  void Model::draw(VkCommandBuffer commandBuffer) {
    vkCmdDraw(commandBuffer, m_VertexCount, 1, 0, 0);
  }

  std::vector<VkVertexInputBindingDescription> Model::Vertex::getBindingDescriptions() {
    std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(Vertex);
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescriptions;
  }

  std::vector<VkVertexInputAttributeDescription> Model::Vertex::getAttributeDescriptions() {
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
    // Position
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, position);

    // Color
    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    return attributeDescriptions;
  }
}
