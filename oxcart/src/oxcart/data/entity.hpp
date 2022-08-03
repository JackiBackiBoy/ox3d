#pragma once

#include "oxcart/core/core.hpp"
#include "oxcart/rendering/model.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

namespace ox {
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
