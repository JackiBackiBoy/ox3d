#pragma once

#include <glm/glm.hpp>

namespace ox {
  struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
  };
}
