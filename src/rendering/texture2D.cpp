#include "texture2D.h"
#include <stb_image.h>
#include <stdexcept>
#include "core/window.h"

void Texture2D::loadFromFile(const std::string& path) {
  int channels;
  int tempWidth;
  int tempHeight;

  std::string enginePath = ENGINE_DIR + path;
  m_Pixels = stbi_load(enginePath.c_str(), &tempWidth, &tempHeight, &channels, STBI_rgb_alpha);

  m_Width = static_cast<uint32_t>(tempWidth);
  m_Height = static_cast<uint32_t>(tempHeight);
  m_Size = m_Width * m_Height * 4;

  if (!m_Pixels) {
    throw std::runtime_error("VULKAN ERROR: Failed to load texture image!");
  }

  GraphicsManager* graphicsManager = Window::currentWindow->getGraphicsManager();
  graphicsManager->submitTexture(this);
}

void Texture2D::cleanup() {
  stbi_image_free(m_Pixels);
}
