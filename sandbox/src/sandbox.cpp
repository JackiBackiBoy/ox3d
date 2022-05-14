#include "oxcart.hpp"

class Sandbox : public ox::Application {
  public:
    Sandbox() : Application::Application() {};
};

ox::Application* targetApp() {
  return new Sandbox();
}
