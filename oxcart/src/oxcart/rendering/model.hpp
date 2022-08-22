#pragma once

#include "oxcart/core/core.hpp"
#include "oxcart/rendering/graphicsDevice.hpp"
#include "oxcart/rendering/buffer.hpp"
#include "oxcart/rendering/mesh.hpp"
#include "oxcart/rendering/texture.hpp"
#include "oxcart/rendering/descriptors.hpp"

#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <vector>
#include <unordered_map>
#include <memory>

namespace ox {
  class OX_API Model {
    public:
      struct OX_API Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec3 bitangent;
        glm::vec2 texCoord;

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
      void draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout);

    private:
      void createVertexBuffers(const std::vector<Vertex>& vertices);
      void createIndexBuffers(const std::vector<uint32_t>& indices);
      void bindTextures();

      void initFromScene(const aiScene* scene);
      void initMeshes(const aiScene* scene);
      void initSingleMesh(const aiMesh* mesh);
      void initMaterials(const aiScene* scene, const std::string& modelDir);
      void countVerticesIndices(const aiScene* scene, uint32_t& numVertices, uint32_t& numIndices);
      void reserveSpace(const uint32_t& numVertices, const uint32_t& numIndices);
      void getTexturePath(const aiMaterial* material, const aiTextureType& texType, aiString& dstPath);

      std::vector<Mesh> m_Meshes;
      std::vector<Texture*> m_DiffuseTextures;
      std::vector<Texture*> m_NormalTextures;
      std::vector<glm::vec3> m_Positions;
      std::vector<glm::vec3> m_Normals;
      std::vector<glm::vec2> m_TexCoords;
      std::vector<uint32_t> m_Indices;
      std::vector<Vertex> m_Vertices;

      std::vector<VkWriteDescriptorSet> m_Writes;
      VkDescriptorPool m_DescriptorPool;
      VkDescriptorPoolSize m_PoolSize;
      std::unique_ptr<DescriptorSetLayout> m_ImageSetLayout;
      GraphicsDevice& m_Device;
      std::vector<VkDescriptorImageInfo> m_ImageInfos;

      std::unique_ptr<Buffer> m_VertexBuffer;
      uint32_t m_VertexCount;

      bool m_HasIndexBuffer = false;
      std::unique_ptr<Buffer> m_IndexBuffer;
      uint32_t m_IndexCount;
  };
}
