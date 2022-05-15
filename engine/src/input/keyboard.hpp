#pragma once

#include "core/core.hpp"
#include "core/window.hpp"
#include "keycode.hpp"

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
