#include "oxcart/rendering/model.hpp"
#include "oxcart/rendering/swapChain.hpp"
#include <cassert>
#include <cstring>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace ox {
  Model::Model(GraphicsDevice& device, const std::vector<Vertex>& vertices)
    : m_Device(device) {
    createVertexBuffers(vertices);
    // TODO: Allow manual assignment of vertex and index buffers
  }

  Model::Model(GraphicsDevice& device, const std::string& path)
    : m_Device(device) {
      loadFromFile(path);
      createVertexBuffers(m_Vertices);
      createIndexBuffers(m_Indices);
      bindTextures();
  }

  Model::~Model() {
    for (size_t i = 0; i < m_Textures.size(); i++) {
      delete m_Textures[i];
    }
  }

  void Model::createVertexBuffers(const std::vector<Vertex>& vertices) {
    m_VertexCount = static_cast<uint32_t>(vertices.size());
    assert(m_VertexCount >= 3 && "VULKAN ASSERTION FAILED: Vertex count must be >= 3");
    VkDeviceSize bufferSize = sizeof(vertices[0]) * m_VertexCount;
    uint32_t vertexSize = sizeof(vertices[0]);

    Buffer stagingBuffer {
      m_Device,
      vertexSize,
      m_VertexCount,
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    };

    stagingBuffer.map();
    stagingBuffer.writeToBuffer((void*)vertices.data());

    m_VertexBuffer = std::make_unique<Buffer>(
        m_Device,
        vertexSize,
        m_VertexCount,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
      );

    m_Device.copyBuffer(stagingBuffer.getBuffer(), m_VertexBuffer->getBuffer(), bufferSize);
  }

  void Model::createIndexBuffers(const std::vector<uint32_t>& indices) {
    m_IndexCount = static_cast<uint32_t>(indices.size());
    m_HasIndexBuffer = m_IndexCount > 0;

    if (!m_HasIndexBuffer) {
      return;
    }

    VkDeviceSize bufferSize = sizeof(indices[0]) * m_IndexCount;
    uint32_t indexSize = sizeof(indices[0]);

    Buffer stagingBuffer = {
      m_Device,
      indexSize,
      m_IndexCount,
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    };

    stagingBuffer.map();
    stagingBuffer.writeToBuffer((void*)indices.data());

    m_IndexBuffer = std::make_unique<Buffer>(
        m_Device,
        indexSize,
        m_IndexCount,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    m_Device.copyBuffer(stagingBuffer.getBuffer(), m_IndexBuffer->getBuffer(), bufferSize);
  }

  void Model::bindTextures() {
    m_PoolSize.resize(m_Meshes.size());
    for (size_t i = 0; i < m_PoolSize.size(); i++) {
      m_PoolSize[i].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
      m_PoolSize[i].descriptorCount = 1;
    }

    VkDescriptorPoolCreateInfo descriptorPool = {};
    descriptorPool.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPool.pNext = NULL;
    descriptorPool.maxSets = m_PoolSize.size();
    descriptorPool.poolSizeCount = m_PoolSize.size();
    descriptorPool.pPoolSizes = m_PoolSize.data();

    vkCreateDescriptorPool(m_Device.device(), &descriptorPool, NULL, &m_DescriptorPool);

    // Allocate descriptor set from the pool
    VkDescriptorSetLayout layouts[1];
    layouts[0] = m_ImageSetLayout->getDescriptorSetLayout();
    std::vector<VkDescriptorSetAllocateInfo> allocInfos(m_PoolSize.size());
    for (size_t i = 0; i < allocInfos.size(); i++) {
      allocInfos[i].sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
      allocInfos[i].pNext = NULL;
      allocInfos[i].descriptorPool = m_DescriptorPool;
      allocInfos[i].descriptorSetCount = 1;
      allocInfos[i].pSetLayouts = layouts;
    }

    writes.resize(m_PoolSize.size());

    for (size_t i = 0; i < m_Meshes.size(); i++) {
      // Create descriptor set for each mesh
      VkDescriptorImageInfo imageInfo{};
      imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      imageInfo.imageView = *m_Textures[m_Meshes[i].materialIndex]->getImageView();
      imageInfo.sampler = SwapChain::m_TextureSampler;

      imageInfos.push_back(imageInfo);

      std::cout << "Material index: " << m_Meshes[i].materialIndex << std::endl;

      vkAllocateDescriptorSets(m_Device.device(), &allocInfos[i], &m_Meshes[i].m_DescriptorSet);
      writes[i] = {};
      writes[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      writes[i].dstSet = m_Meshes[i].m_DescriptorSet; // look into
      writes[i].descriptorCount = 1;
      writes[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
      writes[i].pImageInfo = &imageInfos[i];
      writes[i].dstBinding = 0;
    }

    vkUpdateDescriptorSets(m_Device.device(), writes.size(), writes.data(), 0, NULL);
  }

  void Model::loadFromFile(const std::string& path) {
    std::string enginePath = ENGINE_DIR + path;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(enginePath,
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_MakeLeftHanded | aiProcess_JoinIdenticalVertices);
    if (!scene) {
      std::cout << "ASSIMP ERROR: " << importer.GetErrorString() << std::endl;
      throw std::runtime_error("ASSIMP ERROR: Failed to load model!");
    }

    // Get model directory to locate model resources
    std::string modelDir = enginePath.substr(0, enginePath.find_last_of('/'));
    std::cout << "Model directory: " << modelDir << std::endl;

    initFromScene(scene);
    initMaterials(scene, modelDir);
  }

  void Model::initFromScene(const aiScene* scene) {
    m_Meshes.resize(scene->mNumMeshes);
    m_Textures.resize(scene->mNumMaterials); // TODO: Fix materal count

    m_ImageSetLayout = DescriptorSetLayout::Builder(m_Device)
      .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
      .build();

    std::cout << "Model mesh count: " << scene->mNumMeshes << std::endl;

    //std::cout << "Model material count: " << m_Textures.size() << std::endl;

    // Count number of vertices and indices
    uint32_t numVertices = 0;
    uint32_t numIndices = 0;

    countVerticesIndices(scene, numVertices, numIndices);
    std::cout << "Vertex count: " << numVertices << std::endl;
    std::cout << "Index count: " << numIndices << std::endl;

    // Reserve space for the model data
    reserveSpace(numVertices, numIndices);

    // Initialize all meshes within the model
    initMeshes(scene);
  }

  void Model::initMeshes(const aiScene* scene) {
    for (size_t i = 0; i < m_Meshes.size(); i++) {
      const aiMesh* mesh = scene->mMeshes[i];
      initSingleMesh(mesh);
    }
  }

  void Model::initSingleMesh(const aiMesh* mesh) {
    const aiVector3D zero3D(0.0f, 0.0f, 0.0f);

    // Populate vertices for each mesh
    for (size_t i = 0; i < mesh->mNumVertices; i++) {
      const aiVector3D& position = mesh->mVertices[i];
      const aiVector3D& normal = mesh->mNormals[i];
      const aiVector3D& texCoord = mesh->HasTextureCoords(0) ?
                                   mesh->mTextureCoords[0][i] : zero3D;

      m_Positions.push_back(glm::vec3(position.x, position.y, position.z));
      m_Normals.push_back(glm::vec3(normal.x, normal.y, normal.z));
      m_TexCoords.push_back(glm::vec2(texCoord.x, texCoord.y));

      m_Vertices.push_back(
          { glm::vec3(position.x, position.y, position.z),
            glm::vec3(normal.x, normal.y, normal.z),
            glm::vec2(texCoord.x, texCoord.y) });
    }

    // Populate indices
    for (size_t i = 0; i < mesh->mNumFaces; i++) {
      const aiFace& face = mesh->mFaces[i];
      assert(face.mNumIndices == 3);

      m_Indices.push_back(face.mIndices[0]);
      m_Indices.push_back(face.mIndices[1]);
      m_Indices.push_back(face.mIndices[2]);
    }
  }

  void Model::initMaterials(const aiScene* scene, const std::string& modelDir) {
    for (size_t i = 0; i < scene->mNumMaterials; i++) {
      const aiMaterial* material = scene->mMaterials[i];

      // Check texture count, continue if they exist
      if (material->GetTextureCount(aiTextureType_DIFFUSE) == 0) {
        m_Textures[i] = new Texture(m_Device);
        std::string tempPath = std::string(ENGINE_DIR) + "assets/textures/1x1.png";
        m_Textures[i]->loadFromFile(tempPath);
        std::cout << "Failed at index: " << i << std::endl;
        std::cout << "ASSIMP WARNING: No model textures found!" << std::endl;
      }
      else {
        aiString tempPath;
        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &tempPath, NULL, NULL, NULL, NULL) != AI_SUCCESS) {
          throw std::runtime_error("ASSIMP ERROR: Failed to retrieve model texture!");
        }

        std::string texturePath = modelDir + "/" + std::string(tempPath.data);
        std::cout << "ASSIMP INFO: Loading texture from " << texturePath << std::endl;

        m_Textures[i] = new Texture(m_Device);
        m_Textures[i]->loadFromFile(texturePath);
      }
    }
  }

  void Model::countVerticesIndices(const aiScene* scene, uint32_t& numVertices, uint32_t& numIndices) {
    for (size_t i = 0; i < m_Meshes.size(); i++) {
      m_Meshes[i].materialIndex = static_cast<uint32_t>(scene->mMeshes[i]->mMaterialIndex);
      m_Meshes[i].indices = static_cast<uint32_t>(scene->mMeshes[i]->mNumFaces * 3);
      m_Meshes[i].baseVertex = numVertices; // first vertex of current mesh
      m_Meshes[i].baseIndex = numIndices; // first index of current mesh

      numVertices += static_cast<uint32_t>(scene->mMeshes[i]->mNumVertices);
      numIndices += m_Meshes[i].indices;
    }
  }

  void Model::reserveSpace(const uint32_t& numVertices, const uint32_t& numIndices) {
    m_Positions.reserve(numVertices);
    m_Normals.reserve(numVertices);
    m_TexCoords.reserve(numVertices);
    m_Indices.reserve(numIndices);
  }

  void Model::bind(VkCommandBuffer commandBuffer) {
    VkBuffer buffers[] = {m_VertexBuffer->getBuffer()};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

    if (m_HasIndexBuffer) {
      vkCmdBindIndexBuffer(commandBuffer, m_IndexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
    }
  }

  void Model::draw(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout) {
    for (size_t i = 0; i < m_Meshes.size(); i++) {
      vkCmdBindDescriptorSets(
          commandBuffer,
          VK_PIPELINE_BIND_POINT_GRAPHICS,
          pipelineLayout,
          1,
          1,
          &m_Meshes[i].m_DescriptorSet,
          0,
          nullptr);

      if (m_HasIndexBuffer) {
        vkCmdDrawIndexed(
            commandBuffer,
            m_Meshes[i].indices,
            1,
            m_Meshes[i].baseIndex, m_Meshes[i].baseVertex, 0);
      } else {
        vkCmdDraw(commandBuffer, m_VertexCount, 1, 0, 0);
      }
    }
  }

  std::vector<VkVertexInputBindingDescription> Model::Vertex::getBindingDescriptions() {
    std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(Vertex);
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescriptions;
  }

  std::vector<VkVertexInputAttributeDescription> Model::Vertex::getAttributeDescriptions() {
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions(3);
    // Position
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, position);

    // Normal
    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, normal);

    // Color
    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

    return attributeDescriptions;
  }
}
