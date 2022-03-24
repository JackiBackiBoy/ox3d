#pragma once

#include "core/core.h"
#include <string>

// Singleton
namespace ox {
  class OX_API ResourceManager {
    public:
      static std::string isDefined();

    private:
      ResourceManager() {};
      ~ResourceManager() {};
  };
}
