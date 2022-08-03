#include "oxcart/data/entity.hpp"
#include "oxcart/components/component.hpp"
#include "oxcart/components/transform.hpp"

namespace ox {
  Entity::Entity(id_t id) : m_ID(id) {
    addComponent<Transform>(); // include transform automatically
  }
}
