#pragma once

#include "oxcart/core/core.hpp"
#include "oxcart/rendering/graphicsDevice.hpp"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace ox {
  class OX_API DescriptorSetLayout {
   public:
    class Builder {
     public:
      Builder(GraphicsDevice& device) : m_Device{device} {}

      Builder &addBinding(
          uint32_t binding,
          VkDescriptorType descriptorType,
          VkShaderStageFlags stageFlags,
          uint32_t count = 1);
      std::unique_ptr<DescriptorSetLayout> build() const;

     private:
      GraphicsDevice &m_Device;
      std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings{};
    };

    DescriptorSetLayout(
        GraphicsDevice& device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
    ~DescriptorSetLayout();
    DescriptorSetLayout(const DescriptorSetLayout &) = delete;
    DescriptorSetLayout &operator=(const DescriptorSetLayout &) = delete;

    VkDescriptorSetLayout getDescriptorSetLayout() const { return m_DescriptorSetLayout; }

   private:
    GraphicsDevice &m_Device;
    VkDescriptorSetLayout m_DescriptorSetLayout;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings;

    friend class DescriptorWriter;
  };

  class OX_API DescriptorPool {
   public:
    class Builder {
     public:
      Builder(GraphicsDevice& device) : m_Device{device} {}

      Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
      Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
      Builder &setMaxSets(uint32_t count);
      std::unique_ptr<DescriptorPool> build() const;

     private:
      GraphicsDevice& m_Device;
      std::vector<VkDescriptorPoolSize> m_PoolSizes{};
      uint32_t m_MaxSets = 1000;
      VkDescriptorPoolCreateFlags m_PoolFlags = 0;
    };

    DescriptorPool(
        GraphicsDevice &device,
        uint32_t maxSets,
        VkDescriptorPoolCreateFlags poolFlags,
        const std::vector<VkDescriptorPoolSize> &poolSizes);
    ~DescriptorPool();
    DescriptorPool(const DescriptorPool &) = delete;
    DescriptorPool &operator=(const DescriptorPool &) = delete;

    bool allocateDescriptor(
        const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;

    void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;

    void resetPool();

   private:
    GraphicsDevice &m_Device;
    VkDescriptorPool m_DescriptorPool;

    friend class DescriptorWriter;
  };

  class OX_API DescriptorWriter {
   public:
    DescriptorWriter(DescriptorSetLayout &setLayout, DescriptorPool &pool);

    DescriptorWriter &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
    DescriptorWriter &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);

    bool build(VkDescriptorSet &set);
    void overwrite(VkDescriptorSet &set);

   private:
    DescriptorSetLayout &m_SetLayout;
    DescriptorPool &m_Pool;
    std::vector<VkWriteDescriptorSet> m_Writes;
  };
}
