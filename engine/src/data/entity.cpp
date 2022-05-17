#include "entity.hpp"
#include "components/component.hpp"
#include "components/transform.hpp"

namespace ox {
  Entity::Entity(id_t id) : m_ID(id) {
    addComponent<Transform>(); // include transform automatically
  }
}
