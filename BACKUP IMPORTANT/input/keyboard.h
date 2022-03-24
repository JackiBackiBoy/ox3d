#pragma once

#include "core/core.h"
#include "keyCode.h"

// Singleton
namespace ox {
  class OX_API Keyboard {
    public:
      // Prevent copying or moving
      Keyboard(const Keyboard&) = delete;
      Keyboard(Keyboard&&) = delete;
      Keyboard& operator=(const Keyboard&) = delete;
      Keyboard& operator=(Keyboard&&) = delete;

      static bool isKeyDown(const KeyCode& key);

    private:
      Keyboard() {};
      ~Keyboard() {};
  };
}
