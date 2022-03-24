#pragma once

#include <iostream>
#include "managers/resourceManager.h"
#include "components/camera.h"
#include "ui/uiFont.h"
#include "oxcart.h"
#include <vector>
#include "core/window.h"

extern ox::Window* targetWindow();

int main(void) {
  //Create window
  ox::Window::currentWindow = targetWindow();
  ox::Window::currentWindow->run();

  return 0;
}
