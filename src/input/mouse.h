#pragma once

#include <glm/glm.hpp>

// Singleton
class Mouse {
  public:
    // Prevent copying or moving
    Mouse(const Mouse&) = delete;
    Mouse(Mouse&&) = delete;
    Mouse& operator=(const Mouse&) = delete;
    Mouse& operator=(Mouse&&) = delete;

    static bool isButtonDown(const int& button);
    static glm::vec2 getPosition();

  private:
    Mouse() {};
    ~Mouse() {};
};
