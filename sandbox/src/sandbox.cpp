#include "oxcart/oxcart.hpp"

class Sandbox : public ox::Application {
  private:
    glm::vec2 lastMousePos;
    ox::Camera* playerCamera;
    ox::Transform* playerTransform;

  public:
    Sandbox() : ox::Application::Application() {}

    void onStart() override {
      auto player = ox::Entity::createEntity();
      player.addComponent<ox::Camera>();

      playerCamera = player.getComponent<ox::Camera>();
      ox::Camera::current = playerCamera; // set current camera

      playerTransform = player.getComponent<ox::Transform>();
      std::shared_ptr model = std::make_unique<ox::Model>(m_Device, "assets/models/helmet/FlightHelmet.gltf");

      auto cube = ox::Entity::createEntity();
      cube.model = model;

      auto cubeTransform = cube.getComponent<ox::Transform>();
      cubeTransform->position = { 0.0f, -0.5f, 0.0f };
      cubeTransform->scale = { 0.5f, 0.5f, 0.5f };
      m_Entities.push_back(std::move(cube));

      //auto plane = ox::Entity::createEntity();
      //plane.model = std::make_unique<ox::Model>(m_Device, "assets/models/cube.gltf");
      //plane.getComponent<ox::Transform>()->position = { 0.0f , -2.0f, 0.0f };
      //m_Entities.push_back(std::move(plane));
    }

    void onUpdate(const float& dt) override {
      // ------ Camera movement ------
      // ------ Rotation with mouse ------
      glm::vec2 mousePos = ox::Mouse::getPosition();

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
      if (ox::Keyboard::isKeyDown(ox::Keycode::A, m_Window)) { // left
        playerTransform->position -= camRight * 2.0f * dt;
      }
      if (ox::Keyboard::isKeyDown(ox::Keycode::D, m_Window)) { // right
        playerTransform->position += camRight * 2.0f * dt;
      }

      // Vertical movement (Y-axis)
      if (ox::Keyboard::isKeyDown(ox::Keycode::Space, m_Window)) { // up
        playerTransform->position.y += 2.0f * dt;
      }
      if (ox::Keyboard::isKeyDown(ox::Keycode::LeftControl, m_Window)) { // down
        playerTransform->position.y -= 2.0f * dt;
      }

      // Forward and backward movement (Z-axis)
      if (ox::Keyboard::isKeyDown(ox::Keycode::W, m_Window)) { // forward
        playerTransform->position += newForward * 2.0f * dt;
      }
      if (ox::Keyboard::isKeyDown(ox::Keycode::S, m_Window)) { // backward
        playerTransform->position -= newForward * 2.0f * dt;
      }
    }
};

ox::Application* targetApp() {
  return new Sandbox();
}
