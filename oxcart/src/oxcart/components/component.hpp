#pragma once

#include "oxcart/core/core.hpp"
#include "oxcart/data/entity.hpp"

namespace ox {
  class OX_API Component {
    public:
      Component() {};
      virtual ~Component() {};

      void bindToEntity(Entity* entity) { m_Entity = entity; }

    protected:
      Entity* m_Entity = nullptr;
  };
}
