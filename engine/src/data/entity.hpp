#pragma once

#include "core/core.hpp"
#include "rendering/model.hpp"
#include <memory>

namespace ox {
  struct OX_API Transform {
    glm::vec2 translation{};
    glm::vec2 scale{1.0f, 1.0f};
    float rotation = 0.0f;

    glm::mat2 mat2() {
      const float s = glm::sin(rotation);
      const float c = glm::cos(rotation);

      glm::mat2 rotMat{ { c, s }, { -s, c } };
      glm::mat2 scaleMat{ {scale.x, 0.0f }, { 0.0f, scale.y } };

      return rotMat * scaleMat;
    }
  };

  class OX_API Entity {
    public:
      using id_t = unsigned int;

      static Entity createEntity() {
        static id_t currentId = 0;
        return Entity{currentId++};
      }

      // Delete copy constructor and assign operator
      Entity(const Entity&) = delete;
      Entity& operator=(const Entity&) = delete;
      Entity(Entity&&) = default;
      Entity &operator=(Entity&&) = default;

      inline id_t getID() const { return m_ID; }

      std::shared_ptr<Model> model{};
      glm::vec3 color{};
      Transform transform{};

    private:
      Entity(id_t id) : m_ID(id) {};

      id_t m_ID;
  };
}
