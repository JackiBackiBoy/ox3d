#pragma once

#include "core/core.h"
#include <glm/glm.hpp>

namespace ox {
  struct OX_API UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
  };
}
