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

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    std::cout << "ASSIMP ERROR: " << importer.GetErrorString() << std::endl;
    throw std::runtime_error("ASSIMP ERROR: Failed to load model!");
  }

  // Get model directory to locate model resources
  std::string modelDir = enginePath.substr(0, enginePath.find_last_of('/'));
  std::cout << "Model directory: " << modelDir << std::endl;

  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
  // Process all children nodes recursively
  for (size_t i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);
  }
}
