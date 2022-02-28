#ifndef UNIFORM_BUFFER_OBJECT_HEADER
#define UNIFORM_BUFFER_OBJECT_HEADER

#include <glm/glm.hpp>

struct UniformBufferObject {
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 projection;
};
#endif
