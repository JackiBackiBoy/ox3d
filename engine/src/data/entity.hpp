#pragma once

#include "core/core.hpp"
#include "rendering/model.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

namespace ox {
  struct OX_API TransformBruh {
    glm::vec3 translation{};
    glm::vec3 scale{1.0f, 1.0f, 1.0f};
    glm::vec3 rotation{};

    glm::mat4 mat4() {
      const float c3 = glm::cos(rotation.z);
      const float s3 = glm::sin(rotation.z);
      const float c2 = glm::cos(rotation.x);
      const float s2 = glm::sin(rotation.x);
      const float c1 = glm::cos(rotation.y);
      const float s1 = glm::sin(rotation.y);
      return glm::mat4{
        {
          scale.x * (c1 * c3 + s1 * s2 * s3),
          scale.x * (c2 * s3),
          scale.x * (c1 * s2 * s3 - c3 * s1),
          0.0f,
        },
        {
          scale.y * (c3 * s1 * s2 - c1 * s3),
          scale.y * (c2 * c3),
          scale.y * (c1 * c3 * s2 + s1 * s3),
          0.0f,
        },
        {
          scale.z * (c2 * s1),
          scale.z * (-s2),
          scale.z * (c1 * c2),
          0.0f,
        },
        {translation.x, translation.y, translation.z, 1.0f}};
    }
  };

  class OX_API Component;

  class OX_API Entity {
    public:
      using id_t = unsigned int;

      // Delete copy constructor and assign operator
      Entity(const Entity&) = delete;
      virtual Entity& operator=(const Entity&) = delete;
      Entity(Entity&&) = default;
      virtual Entity &operator=(Entity&&) = default;

      virtual void onStart() {};
      virtual void onUpdate(const float& deltaTime) {};

      template <class T>
      void addComponent() {
        auto result = m_Components.find(std::type_index(typeid(T)));

        // if the component type is not already in m_Components, add it
        if (result == m_Components.end())
        {
          m_Components.insert({ std::type_index(typeid(T)), new T });

          // make sure to bind the component to the entity
          auto entity = static_cast<T*>(m_Components.find(std::type_index(typeid(T)))->second);
          entity->bindToEntity(this);

          return;
        }

        std::cout << "Component already added!" << "\n";
      }

      template <class T>
      T* getComponent() {
        auto result = m_Components.find(std::type_index(typeid(T)));

        // if no component of the given type was found
        if (result == m_Components.end())
        {
          std::cout << "Component not found!" << "\n";
          return nullptr;
        }

        return static_cast<T*>(result->second);
      }

      inline id_t getID() const { return m_ID; }

      std::shared_ptr<Model> model{};
      glm::vec3 color{};

      static Entity createEntity() {
        static id_t currentId = 0;
        return Entity{currentId++};
      }
    private:
      Entity(id_t id);

      std::unordered_map<std::type_index, Component*> m_Components;

      id_t m_ID;
  };
}
