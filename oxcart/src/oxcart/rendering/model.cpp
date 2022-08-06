#include "oxcart/rendering/model.hpp"
#include <cassert>
#include <cstring>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "stb_image.h"

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
      createTextures();
  }

  Model::~Model() {
    // Destroy texture images and views
    vkDestroyImageView(m_Device.device(), m_TextureImageView, nullptr);
    vkDestroyImage(m_Device.device(), m_TextureImage, nullptr);
    vkFreeMemory(m_Device.device(), m_TextureImageMemory, nullptr);
  }

  void Model::createTextures() {
    // Texture image creation
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load("../../assets/models/waterbottle/WaterBottle_baseColor.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels) {
      throw std::runtime_error("VULKAN ERROR: Failed to load texture image!");
    }

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    m_Device.createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(m_Device.device(), stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(m_Device.device(), stagingBufferMemory);

    stbi_image_free(pixels);

    m_Device.createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_TextureImage, m_TextureImageMemory);

    m_Device.transitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    m_Device.copyBufferToImage(stagingBuffer, m_TextureImage, texWidth, texHeight, 1);

    m_Device.transitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    vkDestroyBuffer(m_Device.device(), stagingBuffer, nullptr);
    vkFreeMemory(m_Device.device(), stagingBufferMemory, nullptr);

    // Texture image view creation
    m_TextureImageView = m_Device.createImageView(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB);

    // Texture sampler creation

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

  void Model::loadFromFile(const std::string& path) {
    std::string enginePath = ENGINE_DIR + path;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(enginePath, aiProcess_Triangulate |
                                                         aiProcess_FlipUVs);
    if (!scene) {
      std::cout << "ASSIMP ERROR: " << importer.GetErrorString() << std::endl;
      throw std::runtime_error("ASSIMP ERROR: Failed to load model!");
    }

    // Get model directory to locate model resources
    std::string modelDir = path.substr(0, path.find_last_of('/'));
    std::cout << "Model directory: " << modelDir << std::endl;

    initFromScene(scene);

    //initMaterials(scene, modelDir);
  }

  void Model::initFromScene(const aiScene* scene) {
    m_Meshes.resize(scene->mNumMeshes);
    //m_Textures.resize(scene->mNumMaterials);

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

    // Populate vertices
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

    std::cout << "Normal count: " << m_Normals.size() << "\n";

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
        std::cout << "ASSIMP WARNING: No model textures found!" << std::endl;
        return;
      }

      aiString tempPath;
      if (material->GetTexture(aiTextureType_DIFFUSE, 0, &tempPath, NULL, NULL, NULL, NULL) != AI_SUCCESS) {
        throw std::runtime_error("ASSIMP ERROR: Failed to retrieve model texture!");
      }

      std::string texturePath = modelDir + "/" + std::string(tempPath.data);
      std::cout << "ASSIMP INFO: Loading texture from " << texturePath << std::endl;

      //m_Textures[i] = new Texture2D();
      //m_Textures[i]->loadFromFile(texturePath);
    }
  }

  void Model::countVerticesIndices(const aiScene* scene, uint32_t& numVertices, uint32_t& numIndices) {
    for (size_t i = 0; i < m_Meshes.size(); i++) {
      m_Meshes[i].materialIndex =
        static_cast<uint32_t>(scene->mMeshes[i]->mMaterialIndex);
      m_Meshes[i].indices =
        static_cast<uint32_t>(scene->mMeshes[i]->mNumFaces * 3);
      m_Meshes[i].baseVertex = numVertices;
      m_Meshes[i].baseIndex = numIndices;

      numVertices +=
        static_cast<uint32_t>(scene->mMeshes[i]->mNumVertices);
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

  void Model::draw(VkCommandBuffer commandBuffer) {
    if (m_HasIndexBuffer) {
      vkCmdDrawIndexed(commandBuffer, m_IndexCount, 1, 0, 0, 0);
    } else {
      vkCmdDraw(commandBuffer, m_VertexCount, 1, 0, 0);
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
