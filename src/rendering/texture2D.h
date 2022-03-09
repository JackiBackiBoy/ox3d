#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <string>
#include <vulkan/vulkan.h>

class Texture2D {
  public:
    Texture2D() {};
    
    void loadFromFile(const std::string& path);
    inline int getWidth() { return m_Width; };
    inline int getHeight() { return m_Height; };

  private:
    int m_Width;
    int m_Height;
    VkDeviceSize m_Size;
};
