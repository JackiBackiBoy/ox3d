#pragma once

// Singleton
namespace ox {
  class ResourceManager {
    public:
      ResourceManager(const ResourceManager&) = delete;
      ResourceManager(ResourceManager&&) = delete;
      ResourceManager& operator=(const ResourceManager&) = delete;
      ResourceManager& operator=(ResourceManager&&) = delete;

      void loadResources();

      static ResourceManager& get();

    private:
      ResourceManager() {};
      ~ResourceManager() {};

      static ResourceManager* instance;
  };
}
