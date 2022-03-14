#include "uiFont.h"
#include <stdexcept>

void UIFont::loadFromFile(const std::string& path, const uint32_t& size) {
  std::string enginePath = ENGINE_DIR + path;

  // Load Freetype
  FT_Library ft;

  if (FT_Init_FreeType(&ft)) { // all functions return a value different than 0 whenever an error occurred
    throw std::runtime_error("FREETYPE ERROR: Failed to initialize Freetype library!");
  }

  FT_Face face;
  if (FT_New_Face(ft, enginePath.c_str(), 0, &face)) {
    throw std::runtime_error("FREETYPE ERROR: Failed to load font!");
  }

  // Set font size
  FT_Set_Pixel_Sizes(face, 0, size);

  // Set active glyphs
  if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
    throw std::runtime_error("FREETYPE ERRROR: Failed to load glyph!");
  }
}
