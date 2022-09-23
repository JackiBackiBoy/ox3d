#pragma once

#include "oxcart/core/core.hpp"
#include <vector>

namespace ox {
  class OX_API Renderer2D {
    public:
      Renderer2D() {};
      ~Renderer2D() = default;

      void begin();
      void end();

      void onUpdate();
      void onRender();

    private:
      static constexpr float quadVertices[] = {
        // Triangle 1
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,

        // Triangle 2
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
      };
  };
}
