#include "application.hpp"
#include "rendering/renderSystem.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <iostream>

namespace ox {
  Application::Application() {
    loadEntities();
  }

  Application::~Application() {
  }

  void Application::run() {
    RenderSystem renderSystem{m_Device, m_Renderer.getSwapChainRenderPass()};

    while (!m_Window.shouldClose()) {
      glfwPollEvents();

      if (auto commandBuffer = m_Renderer.beginFrame()) {
        // TODO: begin other render passes

        m_Renderer.beginSwapChainRenderPass(commandBuffer);
        renderSystem.renderEntities(commandBuffer, m_Entities);
        m_Renderer.endSwapChainRenderPass(commandBuffer);
        m_Renderer.endFrame();
      }
    }

    vkDeviceWaitIdle(m_Device.device());
  }

  float sin45 = sin(glm::radians(45.0f));
  std::vector<Model::Vertex> sierpinski(
      const int& iterations,
      const float& sideLen,
      const float& originX,
      const float& originY) {
    float height = sideLen * sin45;

    if (iterations == 0) {
      // Create 3 new equilateral triangles
      // Top triangle
      glm::vec2 a1 = { originX, originY - height / 2.0f }; // top
      glm::vec2 b1 = { originX + sideLen / 4.0f,  originY }; // bottom right
      glm::vec2 c1 = { originX - sideLen / 4.0f,  originY }; // bottom left

      // Bottom right triangle
      glm::vec2 a2 = { originX + sideLen / 4.0f, originY }; // top
      glm::vec2 b2 = { originX + sideLen / 2.0f, originY + height / 2.0f }; // bottom right
      glm::vec2 c2 = { originX, originY + height / 2.0f }; // bottom left

      // Bottom left triangle
      glm::vec2 a3 = { originX - sideLen / 4.0f, originY }; // top
      glm::vec2 b3 = { originX, originY + height / 2.0f }; // bottom right
      glm::vec2 c3 = { originX - sideLen / 2.0f, originY + height / 2.0f }; // bottom left

      std::vector<Model::Vertex> newVertices;
      newVertices.push_back({ a1, { 1.0f, 0.0f, 0.0f } });
      newVertices.push_back({ b1, { 1.0f, 0.0f, 0.0f } });
      newVertices.push_back({ c1, { 1.0f, 0.0f, 0.0f } });

      newVertices.push_back({ a2, { 1.0f, 0.0f, 0.0f } });
      newVertices.push_back({ b2, { 1.0f, 0.0f, 0.0f } });
      newVertices.push_back({ c2, { 1.0f, 0.0f, 0.0f } });

      newVertices.push_back({ a3, { 1.0f, 0.0f, 0.0f } });
      newVertices.push_back({ b3, { 1.0f, 0.0f, 0.0f } });
      newVertices.push_back({ c3, { 1.0f, 0.0f, 0.0f } });

      return newVertices;
    }
    else {
      auto topVertices = sierpinski(iterations - 1, sideLen / 2.0f, originX, originY - height / 4.0f); // top
      auto bottomRightVertices = sierpinski(iterations - 1, sideLen / 2.0f, originX + sideLen / 4.0f, originY + height / 4.0f); // bottom right
      auto bottomLeftVertices = sierpinski(iterations - 1, sideLen / 2.0f, originX - sideLen / 4.0f, originY + height / 4.0f); // bottom left


      std::vector<Model::Vertex> newVertices;
      for (auto t : topVertices) {
        newVertices.push_back(t);
      }

      for (auto br : bottomRightVertices) {
        newVertices.push_back(br);
      }
      for (auto bl : bottomLeftVertices) {
        newVertices.push_back(bl);
      }

      return newVertices;
    }
  }

  void Application::loadEntities() {
    std::vector<Model::Vertex> vertices {
      //{ {  0.0f, -(sideLen * sin45) / 2.0f } },
      //{ {  sideLen / 2.0f,  (sideLen * sin45) / 2.0f } },
      //{ { -sideLen / 2.0f,  (sideLen * sin45) / 2.0f } }
    };

    vertices = sierpinski(4, 1.0f, 0.0f, 0.0f);
    std::cout << vertices.size() << std::endl;

    auto m_Model = std::make_shared<Model>(m_Device, vertices);
    auto triangle = Entity::createEntity();
    triangle.model = m_Model;
    triangle.color = { 0.1f, 0.8f, 0.1f };
    triangle.transform.translation.x = 0.2f;
    triangle.transform.scale = { 2.0f, 0.5f };
    triangle.transform.rotation = -glm::radians(45.0f);
    
    m_Entities.push_back(std::move(triangle));
  }
}
