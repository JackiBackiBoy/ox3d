#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include "core/core.h"
#include <string>
#include <cstdint>
#include <vulkan/vulkan.h>

namespace ox {
  class OX_API Texture2D {
    public:
      Texture2D() {};
      
      void loadFromFile(const std::string& path);
      void cleanup();

      inline uint32_t& getWidth() { return m_Width; };
      inline uint32_t& getHeight() { return m_Height; };
      inline uint64_t& getSize() { return m_Size; };
      inline unsigned char* getPixels() { return m_Pixels; }

      VkImage vkImage;
      VkDeviceMemory vkMemory;

    private:
      uint32_t m_Width;
      uint32_t m_Height;
      uint64_t m_Size;
      unsigned char* m_Pixels;
  };
}
