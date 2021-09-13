#ifndef MESH_HEADER
#define MESH_HEADER

#include <string>
#include <vector>
#include "Vertex.h"

class Mesh
{
public:
	Mesh() {};

	void LoadFromFile(const std::string& filePath);
	inline std::vector<Vertex> GetVertices() { return m_Faces; }

private:
	std::vector<Vertex> m_Faces;
	std::vector<unsigned int> m_Indices;
};
#endif