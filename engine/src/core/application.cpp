#include "application.hpp"
#include "rendering/renderSystem.hpp"
#include "components/camera.hpp"
#include "components/transform.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <chrono>
#include <stdexcept>
#include <iostream>
#include "input/keyboard.hpp"
#include "input/mouse.hpp"

namespace ox {
  Application::Application() {
    loadEntities();
  }

  Application::~Application() {
  }

  void Application::onStart() {

  }

  void Application::onUpdate(const float& dt) {

  }

  void Application::onRender() {

  }

  void Application::run() {
    RenderSystem renderSystem{m_Device, m_Renderer.getSwapChainRenderPass()};

    auto player = Entity::createEntity();
    player.addComponent<Camera>();

    auto playerCamera = player.getComponent<Camera>();
    auto playerTransform = player.getComponent<Transform>();

    //playerCamera->setDirection(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //playerCamera->setTarget(glm::vec3(-1.0f, -2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 2.5f));

    float dt = 0.0f;
    auto currentTime = std::chrono::high_resolution_clock::now();

    while (!m_Window.shouldClose()) {
      glfwPollEvents();

      auto newTime = std::chrono::high_resolution_clock::now();
      float dt =
        std::chrono::duration<float, std::chrono::seconds::period>(
            newTime - currentTime).count();
      currentTime = newTime;


      onUpdate(dt);

      float aspect = m_Renderer.getAspectRatio();
      // Perspective matrix
      playerCamera->setPerspective(80.0f, aspect, 0.1f, 10.0f);


      // ------ Camera movement ------
      // ------ Rotation with mouse ------
      glm::vec2 mousePos = Mouse::getPosition();

      // Fix for preventing violent camera rotation before
      // or during the initial movement with the mouse
      glm::vec2 deltaMousePos = mousePos - lastMousePos;
      lastMousePos = mousePos;

      playerCamera->setYaw(playerCamera->getYaw() - 0.001f * deltaMousePos.x);
      playerCamera->setPitch(playerCamera->getPitch() - 0.001f * deltaMousePos.y);

      // Rotations
      glm::vec3 newForward{
        glm::cos(playerCamera->getYaw()) * glm::cos(playerCamera->getPitch()),
        glm::sin(playerCamera->getPitch()),
        glm::sin(playerCamera->getYaw()) * glm::cos(playerCamera->getPitch())
      };

      glm::vec3 camForward = glm::normalize(newForward);
      glm::vec3 camRight = glm::normalize(glm::cross({0.0f, 1.0f, 0.0f}, camForward));
      glm::vec3 camUp = glm::normalize(glm::cross(camRight, camForward));

      // View matrix
      playerCamera->m_ViewMatrix = glm::lookAt(
          playerTransform->position,
          playerTransform->position + camForward,
          camUp);

      // Horizontal movement (X-axis)
      if (Keyboard::isKeyDown(Keycode::A, m_Window)) { // left
        playerTransform->position -= camRight * 10.0f * dt;
      }
      if (Keyboard::isKeyDown(Keycode::D, m_Window)) { // right
        playerTransform->position += camRight * 10.0f * dt;
      }

      // Vertical movement (Y-axis)
      if (Keyboard::isKeyDown(Keycode::Space, m_Window)) { // up
        playerTransform->position.y += 10.0f * dt;
      }
      if (Keyboard::isKeyDown(Keycode::LeftControl, m_Window)) { // down
        playerTransform->position.y -= 10.0f * dt;
      }

      // Forward and backward movement (Z-axis)
      if (Keyboard::isKeyDown(Keycode::W, m_Window)) { // forward
        playerTransform->position += newForward * 10.0f * dt;
      }
      if (Keyboard::isKeyDown(Keycode::S, m_Window)) { // backward
        playerTransform->position -= newForward * 10.0f * dt;
      }

      if (auto commandBuffer = m_Renderer.beginFrame()) {
        // TODO: begin other render passes

        m_Renderer.beginSwapChainRenderPass(commandBuffer);
        renderSystem.renderEntities(commandBuffer, m_Entities, *playerCamera);
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

    auto cubeTransform = cube.getComponent<Transform>();
    cubeTransform->position = { 0.0f, 0.0f, 2.5f };
    cubeTransform->scale = { 0.5f, 0.5f, 0.5f };
    m_Entities.push_back(std::move(cube));
  }
}
