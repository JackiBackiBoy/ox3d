#pragma once

#include "oxcart/core/core.hpp"
#include "oxcart/components/component.hpp"

namespace ox {
  class OX_API Light : public Component {
    public:
      Light();

      glm::vec3 color{ 1.0f, 1.0f, 1.0f };
      float radius{ 1.0f };
      float intensity{ 5.0f };
  };
}
