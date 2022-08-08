#include "oxcart/rendering/texture.hpp"
#include "oxcart/rendering/buffer.hpp"
#include "stb_image.h"

// std
#include <stdexcept>

namespace ox {
  Texture::~Texture() {
    // Destroy texture images and views
    vkDestroyImageView(m_Device.device(), m_TextureImageView, nullptr);
    vkDestroyImage(m_Device.device(), m_TextureImage, nullptr);
    vkFreeMemory(m_Device.device(), m_TextureImageMemory, nullptr);
  }

  void Texture::loadFromFile(const std::string& path) {
    // Texture image creation
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels) {
      throw std::runtime_error("VULKAN ERROR: Failed to load texture image!");
    }

    void* data;
    Buffer stagingBuffer = {
        m_Device,
        imageSize,
        1,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        0
    };

    stagingBuffer.map(imageSize);
    stagingBuffer.writeToBuffer(pixels);
    stagingBuffer.unmap();

    stbi_image_free(pixels);
    m_Device.createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_TextureImage, m_TextureImageMemory);

    m_Device.transitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
      m_Device.copyBufferToImage(stagingBuffer.getBuffer(), m_TextureImage, texWidth, texHeight, 1);
    m_Device.transitionImageLayout(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    // Texture image view creation
    m_TextureImageView = m_Device.createImageView(m_TextureImage, VK_FORMAT_R8G8B8A8_SRGB);

  }
}
