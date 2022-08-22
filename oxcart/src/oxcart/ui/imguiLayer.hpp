#pragma once

#include "oxcart/core/core.hpp"
#include "oxcart/rendering/graphicsDevice.hpp"
#include "oxcart/rendering/renderer.hpp"
#include "oxcart/rendering/frameInfo.hpp"
#include <vulkan/vulkan.h>

namespace ox {
  class OX_API ImGuiLayer {
    public:
      // Delete copy constructor and assign operator
      ImGuiLayer(const ImGuiLayer&) = delete;
      ImGuiLayer& operator=(const ImGuiLayer&) = delete;
      ImGuiLayer(ImGuiLayer&&) = default;
      ImGuiLayer &operator=(ImGuiLayer&&) = default;
      
      static void initialize(GraphicsDevice& device, Renderer& renderer);
      static void render(FrameInfo& frameInfo);
      static bool isUsingMouse();
      static bool isUsingKeyboard();

    private:
      static VkDescriptorPool m_Pool;
      ImGuiLayer() = default;
  };
}