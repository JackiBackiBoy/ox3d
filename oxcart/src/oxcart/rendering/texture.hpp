#pragma once

#include "oxcart/core/core.hpp"
#include "oxcart/rendering/graphicsDevice.hpp"
#include <string>

#define STB_IMAGE_IMPLEMENTATION

namespace ox {
  class OX_API Texture {
    public:
      Texture(GraphicsDevice& device) : m_Device(device) {};
      ~Texture();

      void loadFromFile(const std::string& path);
      VkImageView* getImageView() { return &m_TextureImageView; }

    private:
      GraphicsDevice& m_Device;
      VkImage m_TextureImage;
      VkDeviceMemory m_TextureImageMemory;
      VkImageView m_TextureImageView;
  };
}
