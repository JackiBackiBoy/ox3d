#pragma once

#include <cstdint>

struct Mesh {
  Mesh() {};
  ~Mesh() {};

  uint32_t indices;
  uint32_t materialIndex;
  uint32_t baseVertex;
  uint32_t baseIndex;
};
