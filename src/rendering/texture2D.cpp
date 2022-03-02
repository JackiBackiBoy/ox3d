#include "texture2D.h"
#include <stb_image.h>
#include <stdexcept>

void Texture2D::loadFromFile(const std::string& path) {
  int channels;

  std::string enginePath = ENGINE_DIR + path;
  stbi_uc* pixels = stbi_load(enginePath.c_str(), &m_Width, &m_Height, &channels, STBI_rgb_alpha);
  m_Size = m_Width * m_Height * 4;

  if (!pixels) {
    throw std::runtime_error("VULKAN ERROR: Failed to load texture image!");
  }

  
}
