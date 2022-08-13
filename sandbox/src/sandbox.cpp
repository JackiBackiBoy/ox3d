#include "oxcart/oxcart.hpp"

class Sandbox : public ox::Application {
  private:
    bool firstMove = false;
    glm::vec2 lastMousePos;
    ox::Camera* playerCamera;
    ox::Transform* playerTransform;
    ox::Entity* light;
    ox::Entity* light2;

  public:
    Sandbox() : ox::Application::Application() {}

    void onStart() override {
      auto player = ox::Entity::createEntity();
      player->addComponent<ox::Camera>();


      playerCamera = player->getComponent<ox::Camera>();
      ox::Camera::current = playerCamera; // set current camera

      playerTransform = player->getComponent<ox::Transform>();
      std::shared_ptr model = std::make_unique<ox::Model>(m_Device, "assets/models/helmet/FlightHelmet.gltf");

      auto helmet = ox::Entity::createEntity();
      helmet->model = model;

      auto cubeTransform = helmet->getComponent<ox::Transform>();
      cubeTransform->position = { 0.0f, 1.3f, 0.0f };
      m_Entities.push_back(helmet);

      auto plane = ox::Entity::createEntity();
      plane->model = std::make_unique<ox::Model>(m_Device, "assets/models/cube.gltf");
      plane->getComponent<ox::Transform>()->position = { 0.0f , -3.2f, 0.0f };
      plane->getComponent<ox::Transform>()->scale = { 3.0f , 3.0f, 3.0f };
      m_Entities.push_back(plane);

      // Make point lights
      light = ox::Entity::createEntity();
      light->getComponent<ox::Transform>()->position = { 1.0f, 1.0f, -3.0f };
      auto lightComponent = light->addComponent<ox::Light>();
      lightComponent->color = { 1.0f, 1.0f, 1.0f };
      lightComponent->intensity = 0.5f;
      lightComponent->radius = 0.09f;
      m_Entities.push_back(light);

      light2 = ox::Entity::createEntity();
      light2->getComponent<ox::Transform>()->position = { 1.0f, 1.0f, -3.0f };
      auto lightComponent2 = light2->addComponent<ox::Light>();
      lightComponent2->color = { 1.0f, 1.0f, 1.0f };
      lightComponent2->intensity = 0.5f;
      lightComponent2->radius = 0.09f;
      m_Entities.push_back(light2);
    }

    void onUpdate(const float& dt) override {
      static float totalTime;
      totalTime += dt;

      light->getComponent<ox::Transform>()->position.x = cos(totalTime);
      light->getComponent<ox::Transform>()->position.z = sin(totalTime);

      light2->getComponent<ox::Transform>()->position.x = sin(totalTime);
      light2->getComponent<ox::Transform>()->position.y = 1.8f;
      light2->getComponent<ox::Transform>()->position.z = -cos(totalTime);

      // ------ Camera movement ------
      // ------ Rotation with mouse ------
      glm::vec2 mousePos = ox::Mouse::getPosition();

      // Fix for preventing violent camera rotation before
      // or during the initial movement with the mouse
      glm::vec2 deltaMousePos = mousePos - lastMousePos;
      lastMousePos = mousePos;

      if (!firstMove) {
        deltaMousePos = { 0, 0 };
        firstMove = true;
      }

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

      playerCamera->onUpdate(playerTransform->position);

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
