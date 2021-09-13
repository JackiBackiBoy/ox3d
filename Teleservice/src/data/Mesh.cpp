#include "Mesh.h"
#include <fstream>
#include <sstream>

void Mesh::LoadFromFile(const std::string& filePath)
{
	std::ifstream file;
	file.open(filePath);

	std::vector<Vector3D> triangles;
	std::vector<Vector3D> normals;

	if (file.is_open())
	{
		std::string line;

		while (std::getline(file, line))
		{
			std::istringstream lineStream(line);

			std::string type;
			lineStream >> type;

			if (type == "v")
			{
				Vector3D position;
				lineStream >> position.x >> position.y >> position.z;
				triangles.push_back(position);
			}
			else if (type == "vn")
			{
				Vector3D normal;
				lineStream >> normal.x >> normal.y >> normal.z;
				normals.push_back(normal);
			}
			else if (type == "f")
			{
				char junk;

				unsigned int posIndexA;
				unsigned int posIndexB;
				unsigned int posIndexC;

				unsigned int normalIndexA;
				unsigned int normalIndexB;
				unsigned int normalIndexC;

				lineStream >> posIndexA >> junk >> junk >> normalIndexA >> posIndexB >> junk >> junk >> normalIndexB >> posIndexC >> junk >> junk >> normalIndexC;

				Vertex vertex1;
				vertex1.position = triangles[posIndexA - 1];
				vertex1.normal = normals[normalIndexA - 1];

				Vertex vertex2;
				vertex2.position = triangles[posIndexB - 1];
				vertex2.normal = normals[normalIndexB - 1];

				Vertex vertex3;
				vertex3.position = triangles[posIndexC - 1];
				vertex3.normal = normals[normalIndexC - 1];

				m_Faces.push_back(vertex1);
				m_Faces.push_back(vertex2);
				m_Faces.push_back(vertex3);
			}
		}

		file.close();
	}


}
