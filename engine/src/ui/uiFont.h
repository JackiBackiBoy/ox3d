#pragma once

#include "core/core.h"
#include <string>
#include <cstdint>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace ox {
  class OX_API UIFont {
    public:
      UIFont() {};
      ~UIFont() {};

      void loadFromFile(const std::string& path, const uint32_t& size);
  };
}
