#pragma once

#include <string>
#include <vector>
#include <assimp/scene.h>
#include "mesh.h"
#include <glm/glm.hpp>
#include "vertex.h"

class Model {
  public:
    Model() {};

    void loadFromFile(const std::string& path);
    void initFromScene(const aiScene* scene);
    void countVerticesAndIndices(const aiScene* scene, uint32_t& numVertices,
                                 uint32_t& numIndices);
    void reserveSpace(const uint32_t& numVertices, const uint32_t& numIndices);
    void initMeshes(const aiScene* scene);
    void initSingleMesh(const aiMesh* mesh);
    void processNode(aiNode* node, const aiScene* scene);

    // Getters
    inline std::vector<Vertex>& getVertices() { return m_Vertices; }
    inline std::vector<uint32_t>& getIndices() { return m_Indices; }

  private:
    std::vector<Mesh> m_Meshes;
    std::vector<glm::vec3> m_Positions;
    std::vector<glm::vec3> m_Normals;
    std::vector<glm::vec2> m_TexCoords;
    std::vector<uint32_t> m_Indices;
    std::vector<Vertex> m_Vertices;
};
