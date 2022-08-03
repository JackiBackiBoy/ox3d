#pragma once

#include "oxcart/core/core.hpp"
#include "oxcart/core/window.hpp"
#include "oxcart/input/keycode.hpp"

namespace ox {
  class OX_API Keyboard {
    public:
      // Delete copy constructor and assign operator
      Keyboard(const Keyboard&) = delete;
      Keyboard& operator=(const Keyboard&) = delete;
      Keyboard(Keyboard&&) = default;
      Keyboard &operator=(Keyboard&&) = default;

      static bool isKeyDown(const Keycode& key, Window& window);

    private:
      Keyboard() {};
  };
}
