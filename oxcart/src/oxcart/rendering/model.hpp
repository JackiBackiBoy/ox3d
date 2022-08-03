#pragma once

#include "oxcart/core/core.hpp"
#include "oxcart/rendering/graphicsDevice.hpp"
#include "oxcart/rendering/mesh.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <assimp/scene.h>

#include <vector>

namespace ox {
  class OX_API Model {
    public:
      struct OX_API Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 color;

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
      };

      Model(GraphicsDevice& device, const std::vector<Vertex>& vertices);
      Model(GraphicsDevice& device, const std::string& path);
      ~Model();

      // Delete copy constructors
      Model(const Model&) = delete;
      Model &operator=(const Model&) = delete;

      void loadFromFile(const std::string& path);
      void bind(VkCommandBuffer commandBuffer);
      void draw(VkCommandBuffer commandBuffer);

    private:
      void createVertexBuffers(const std::vector<Vertex>& vertices);
      void createIndexBuffers(const std::vector<uint32_t>& indices);

      void initFromScene(const aiScene* scene);
      void initMeshes(const aiScene* scene);
      void initSingleMesh(const aiMesh* mesh);
      void initMaterials(const aiScene* scene, const std::string& modelDir);
      void countVerticesIndices(const aiScene* scene, uint32_t& numVertices, uint32_t& numIndices);
      void reserveSpace(const uint32_t& numVertices, const uint32_t& numIndices);

      std::vector<Mesh> m_Meshes;
      //std::vector<Texture*> m_Textures;
      std::vector<glm::vec3> m_Positions;
      std::vector<glm::vec3> m_Normals;
      std::vector<glm::vec2> m_TexCoords;
      std::vector<uint32_t> m_Indices;
      std::vector<Vertex> m_Vertices;

      GraphicsDevice& m_Device;

      bool m_HasIndexBuffer = false;
      VkBuffer m_VertexBuffer;
      VkDeviceMemory m_VertexBufferMemory;
      uint32_t m_VertexCount;
      VkBuffer m_IndexBuffer;
      VkDeviceMemory m_IndexBufferMemory;
      uint32_t m_IndexCount;
  };
}
