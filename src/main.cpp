#include <iostream>

#include "core/window.h"

int main() {
  // Create window
  Window* window = new Window(500, 500);
  window->run();

  // Delete window
  window = nullptr;
  delete window;

  return 0;
}
