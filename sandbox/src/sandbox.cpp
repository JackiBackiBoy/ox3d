#include <oxcart/oxcart.hpp>

class Sandbox : public ox::Application {
  public:
    Sandbox() : Application::Application() {
      //auto player = ox::Entity::createEntity();
      //player.addComponent<ox::Camera>();

      //m_Entities.push_back(player);
    }
};

ox::Application* targetApp() {
  return new Sandbox();
}
