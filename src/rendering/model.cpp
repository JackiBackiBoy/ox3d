#include "model.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>
#include <stdexcept>

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

  initMaterials(scene, modelDir);
}

void Model::initFromScene(const aiScene* scene) {
  m_Meshes.resize(scene->mNumMeshes);
  m_Textures.resize(scene->mNumMaterials);

  std::cout << "Model material count: " << m_Textures.size() << std::endl;

  // Count number of vertices and indices
  uint32_t numVertices = 0;
  uint32_t numIndices = 0;
  countVerticesAndIndices(scene, numVertices, numIndices);
  std::cout << "Vertex count: " << numVertices << std::endl;
  std::cout << "Index count: " << numIndices << std::endl;

  // Reserve space for the model data
  reserveSpace(numVertices, numIndices);

  // Initialize all meshes within the model
  initMeshes(scene);
}

void Model::countVerticesAndIndices(const aiScene* scene, uint32_t& numVertices,
                                    uint32_t& numIndices) {
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

    m_Vertices.push_back({ glm::vec3(position.x, position.y, position.z),
                           glm::vec3(1.0f, 1.0f, 1.0f),
                           glm::vec2(texCoord.x, texCoord.y)});
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
      std::cout << "ASSIMP WARNING: No model textures found!" << std::endl;
      return;
    }

    aiString tempPath;
    if (material->GetTexture(aiTextureType_DIFFUSE, 0, &tempPath, NULL, NULL, NULL, NULL) != AI_SUCCESS) {
      throw std::runtime_error("ASSIMP ERROR: Failed to retrieve model texture!");
    }

    std::string texturePath = modelDir + "/" + std::string(tempPath.data);
    std::cout << "ASSIMP INFO: Loading texture from " << texturePath << std::endl;

    m_Textures[i] = new Texture2D();
    m_Textures[i]->loadFromFile(texturePath);
  }
}
