#include "application.hpp"
#include "rendering/renderSystem.hpp"
#include "components/camera.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <chrono>
#include <stdexcept>
#include <iostream>
#include "input/keyboard.hpp"

namespace ox {
  Application::Application() {
    loadEntities();
  }

  Application::~Application() {
  }

  void Application::run() {
    RenderSystem renderSystem{m_Device, m_Renderer.getSwapChainRenderPass()};
    Camera camera{};
    //camera.setDirection(glm::vec3(0.0f), glm::vec3(0.5f, 0.1, 1.0f));
    camera.setTarget(glm::vec3(-1.0f, -2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 2.5f));

    float deltaTime = 0.0f;
    auto currentTime = std::chrono::high_resolution_clock::now();

    while (!m_Window.shouldClose()) {
      glfwPollEvents();

      auto newTime = std::chrono::high_resolution_clock::now();
      float deltaTime =
        std::chrono::duration<float, std::chrono::seconds::period>(
            newTime - currentTime).count();
      currentTime = newTime;

      float aspect = m_Renderer.getAspectRatio();
      camera.setPerspective(glm::radians(60.0f), aspect, 0.1f, 10.0f);

      // Camera movement
      glm::vec3 rotate{};

      if (Keyboard::isKeyDown(Keycode::Q, m_Window)) {
        rotate.y += 1.0f;
      }
      if (Keyboard::isKeyDown(Keycode::E, m_Window)) {
        rotate.y -= 1.0f;
      }

      if (auto commandBuffer = m_Renderer.beginFrame()) {
        // TODO: begin other render passes

        m_Renderer.beginSwapChainRenderPass(commandBuffer);
        renderSystem.renderEntities(commandBuffer, m_Entities, camera);
        m_Renderer.endSwapChainRenderPass(commandBuffer);
        m_Renderer.endFrame();
      }
    }

    vkDeviceWaitIdle(m_Device.device());
  }

  std::unique_ptr<Model> createCubeModel(GraphicsDevice& device, glm::vec3 offset) {
  std::vector<Model::Vertex> vertices{
      // left face (white)
      {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
      {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
      {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

      // right face (yellow)
      {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
      {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
      {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

      // top face (orange, remember y axis points down)
      {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
      {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
      {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

      // bottom face (red)
      {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
      {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
      {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

      // nose face (blue)
      {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
      {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
      {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

      // tail face (green)
      {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
      {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
      {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
    };

    for (auto& v : vertices) {
      v.position += offset;
    }
    return std::make_unique<Model>(device, vertices);
  }

  void Application::loadEntities() {
    std::shared_ptr<Model> model = createCubeModel(m_Device, { 0.0f, 0.0f, 0.0f });

    auto cube = Entity::createEntity();
    cube.model = model;
    cube.transform.translation = { 0.0f, 0.0f, 2.5f };
    cube.transform.scale = { 0.5f, 0.5f, 0.5f };
    m_Entities.push_back(std::move(cube));
  }
}
