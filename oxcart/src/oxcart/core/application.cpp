#include "oxcart/core/application.hpp"
#include "oxcart/rendering/renderSystem.hpp"
#include "oxcart/components/camera.hpp"
#include "oxcart/components/transform.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <chrono>
#include <stdexcept>
#include <iostream>
#include "oxcart/input/keyboard.hpp"
#include "oxcart/input/mouse.hpp"

namespace ox {
  Application::Application() {
  }

  Application::~Application() {
  }

  void Application::onStart() {
  }

  void Application::onUpdate(const float& dt) {
      float aspect = m_Renderer.getAspectRatio();

      if (aspect != lastAspectRatio) {
        // Perspective matrix
        Camera::current->setPerspective(glm::radians(80.0f), aspect, 0.01f, 100.0f);
        lastAspectRatio = aspect;
      }
  }

  void Application::onRender() {

  }

  void Application::run() {
    Application::onStart();
    onStart();

    RenderSystem renderSystem{m_Device, m_Renderer.getSwapChainRenderPass()};

    float dt = 0.0f;
    float lastAspectRatio = 0;
    auto currentTime = std::chrono::high_resolution_clock::now();

    while (!m_Window.shouldClose()) {
      glfwPollEvents();

      auto newTime = std::chrono::high_resolution_clock::now();
      float dt =
        std::chrono::duration<float, std::chrono::seconds::period>(
            newTime - currentTime).count();
      currentTime = newTime;

      Application::onUpdate(dt);
      onUpdate(dt);

      if (auto commandBuffer = m_Renderer.beginFrame()) {
        // TODO: begin other render passes

        m_Renderer.beginSwapChainRenderPass(commandBuffer);
        renderSystem.renderEntities(commandBuffer, m_Entities, *Camera::current);
        m_Renderer.endSwapChainRenderPass(commandBuffer);
        m_Renderer.endFrame();
      }
    }

    vkDeviceWaitIdle(m_Device.device());
  }
}
