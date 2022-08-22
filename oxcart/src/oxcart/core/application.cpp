#include "oxcart/core/application.hpp"
#include "oxcart/rendering/buffer.hpp"
#include "oxcart/components/camera.hpp"
#include "oxcart/components/transform.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <chrono>
#include <stdexcept>
#include <iostream>
#include "oxcart/input/keyboard.hpp"
#include "oxcart/input/mouse.hpp"
#include "oxcart/ui/imguiLayer.hpp"

namespace ox {
  Application::Application() {
    globalPool = DescriptorPool::Builder(m_Device)
      .setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT) // number of descriptor sets
      .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
      .build();
  }

  Application::~Application() {
  }

  void Application::onStart() {
    m_GlobalDescriptorSets.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

    // Fix for NonCoherentAtomSize bug when flushing memory of UBO
    m_UboBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

    // Global uniform buffer (GlobalUBO object)
    for (int i = 0; i < m_UboBuffers.size(); i++) {
      m_UboBuffers[i] = std::make_unique<Buffer>(
          m_Device,
          sizeof(GlobalUBO),
          1,
          VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
      m_UboBuffers[i]->map();
    }

    auto globalSetLayout = DescriptorSetLayout::Builder(m_Device)
      .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
      .build();

    auto imageSetLayout = DescriptorSetLayout::Builder(m_Device)
      .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
      .addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
      .build();

    for (int i = 0; i < m_GlobalDescriptorSets.size(); i++) {
      auto bufferInfo = m_UboBuffers[i]->descriptorInfo();
      DescriptorWriter(*globalSetLayout, *globalPool)
        .writeBuffer(0, &bufferInfo)
        .build(m_GlobalDescriptorSets[i]);
    }

    std::vector<VkDescriptorSetLayout> setLayouts = {
      globalSetLayout->getDescriptorSetLayout(),
      imageSetLayout->getDescriptorSetLayout()
    };

    // Systems
    m_RenderSystem = std::make_unique<RenderSystem>(
      m_Device,
      m_Renderer.getSwapChainRenderPass(),
      setLayouts);

    m_PointLightSystem = std::make_unique<PointLightSystem>(
      m_Device,
      m_Renderer.getSwapChainRenderPass(),
      setLayouts);
  }

  void Application::onUpdate(const float& dt) {
    float aspect = m_Renderer.getAspectRatio();

    if (aspect != m_LastAspectRatio) {
      // Perspective matrix
      Camera::current->setPerspective(glm::radians(80.0f), aspect, 0.01f, 100.0f);
      m_LastAspectRatio = aspect;
    }
  }

  void Application::onRender(const float& dt) {
    if (auto commandBuffer = m_Renderer.beginFrame()) {
      int frameIndex = m_Renderer.getFrameIndex();

      std::vector<VkDescriptorSet> descriptorSets = {
        m_GlobalDescriptorSets[frameIndex],
      };

      FrameInfo frameInfo {
        frameIndex,
        dt,
        commandBuffer,
        *Camera::current,
        descriptorSets,
        m_Entities
      };

      // Update
      GlobalUBO ubo{};
      ubo.projection = Camera::current->getProjection();
      ubo.view = Camera::current->getView();
      ubo.inverseView = Camera::current->getInverseView();

      m_PointLightSystem->onUpdate(frameInfo, ubo);
      m_UboBuffers[frameIndex]->writeToBuffer(&ubo);
      m_UboBuffers[frameIndex]->flush();
      
      // Render
      m_Renderer.beginSwapChainRenderPass(commandBuffer);

      m_RenderSystem->renderEntities(frameInfo, m_Entities);
      m_PointLightSystem->render(frameInfo);
      ImGuiLayer::render(frameInfo);

      m_Renderer.endSwapChainRenderPass(commandBuffer);
      m_Renderer.endFrame();
    }
  }

  void Application::run() {
    Application::onStart();
    onStart();

    ImGuiLayer::initialize(m_Device, m_Renderer);

    float dt = 0.0f;
    auto currentTime = std::chrono::high_resolution_clock::now();

    // Application loop
    while (!m_Window.shouldClose()) {
      glfwPollEvents();

      // Delta time
      auto newTime = std::chrono::high_resolution_clock::now();
      float dt = std::chrono::duration<float, std::chrono::seconds::period>(
          newTime - currentTime).count();
      currentTime = newTime;

      Application::onUpdate(dt);
      onUpdate(dt);

      onRender(dt);
      Application::onRender(dt);
    }

    vkDeviceWaitIdle(m_Device.device());
  }
}
