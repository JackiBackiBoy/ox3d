#pragma once

#include "core/application.hpp"
#include <cstdlib>
#include <iostream>
#include <stdexcept>

extern ox::Application* targetApp();

int main(void) {
  ox::Application* app = targetApp();

  try {
    app->run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
