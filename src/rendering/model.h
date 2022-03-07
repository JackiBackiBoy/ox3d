#ifndef MODEL_HEADER
#define MODEL_HEADER

#include <string>
#include <assimp/scene.h>

class Model {
  public:
    Model() {};

    void loadFromFile(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);

  private:

};
#endif
