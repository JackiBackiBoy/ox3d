#include "resourceManager.h"

using namespace ox;

ResourceManager& ResourceManager::get() {
  static ResourceManager rm;
  return rm;
}
