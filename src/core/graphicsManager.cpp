#include "graphicsManager.h"
#include <stdexcept>
#include <vector>
#include <iostream>

void GraphicsManager::loadVulkan() {
  // ------ Application information ------
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = m_Window->getTitle().c_str();
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  // ------ Vulkan Creation Info ------
  // Tell vulkan which extensions and validation layers to use
  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  // Get number of required GLFW extensions
  uint32_t glfwExtensionCount = 0;
  const char** glfwExtensions;

  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  // Add GLFW extension count and link to the required extensions
  createInfo.enabledExtensionCount = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames = glfwExtensions;
  createInfo.enabledLayerCount = 0;
  
  // ------ Create Vulkan Instance ------
  if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS) {
    throw std::runtime_error("VULKAN ERROR: Failed to create VkInstance!");
  }

  // ------ Check Extension Support ------
  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

  // Store all extensions in extensions vector
  std::vector<VkExtensionProperties> extensions(extensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

  // List all supported extensions
  std::cout << "Extensions supported: " << extensionCount << "\n";
  for (const auto& extension : extensions) {
    std::cout << "\t" << extension.extensionName << "\n";
  }
}

void GraphicsManager::destroyVulkan() {
  vkDestroyInstance(m_Instance, nullptr);
}
