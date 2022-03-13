#pragma once

#include "keyCode.h"

// Singleton
class Keyboard {
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
