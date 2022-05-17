#pragma once

#include "core/core.hpp"
#include "data/entity.hpp"

namespace ox {
  class OX_API Component {
    public:
      Component() {};
      virtual ~Component() {};

      virtual void onStart() = 0;
      virtual void onUpdate(const float& deltaTime) = 0;

      inline void bindToEntity(Entity* entity) { m_Entity = entity; }

    protected:
      Entity* m_Entity;
  };
}
