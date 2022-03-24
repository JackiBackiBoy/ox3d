#include "oxcart.h"

class Sandbox : public ox::Window {
  public:
    Sandbox() : ox::Window(1280, 720, "Oxcart Vulkan renderer") {};

    void onStart() override {

    }
    void onUpdate(const float& deltaTime) override {

    }
    void onRender() override {

    }
};

ox::Window* targetWindow() {
  return new Sandbox();
}
