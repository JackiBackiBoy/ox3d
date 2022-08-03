#pragma once

#include "oxcart/core/core.hpp"
#include <cstdint>

namespace ox {
  struct OX_API Mesh {
    Mesh() {};
    ~Mesh() {};

    uint32_t indices;
    uint32_t materialIndex;
    uint32_t baseVertex;
    uint32_t baseIndex;
  };
}
