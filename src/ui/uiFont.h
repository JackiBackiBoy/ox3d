#pragma once

#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H

class UIFont {
  public:
    UIFont() {};
    ~UIFont() {};

    void loadFromFile(const std::string& path);
};
