#pragma once

#include "core/core.hpp"
#include "rendering/model.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

namespace ox {
  struct OX_API Transform {
    glm::vec3 translation{};
    glm::vec3 scale{1.0f, 1.0f, 1.0f};
    glm::vec3 rotation{};

    glm::mat4 mat4() {
      auto transform = glm::translate(glm::mat4{1.0f}, translation);

      transform = glm::rotate(transform, rotation.y, { 0.0f, 1.0f, 0.0f }); // yaw
      transform = glm::rotate(transform, rotation.x, { 1.0f, 0.0f, 0.0f }); // pitch
      transform = glm::rotate(transform, rotation.z, { 0.0f, 0.0f, 1.0f }); // roll

      transform = glm::scale(transform, scale);

      return transform;
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
