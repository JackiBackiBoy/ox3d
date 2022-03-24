#pragma once

#define GLFW_INCLUDE_VULKAN

#include "core.h"
#include <cstdint>
#include <string>
#include <GLFW/glfw3.h>
#include "rendering/shader.h"
#include "graphicsManager.h"
#include "components/camera.h"

namespace ox {
  class OX_API Window {
    public:
      Window(const uint32_t& width, const uint32_t& height, const std::string& title);
      virtual ~Window() {};

      virtual void onStart();
      virtual void onUpdate(const float& deltaTime);
      virtual void onRender();
      void run();

      // Getters
      inline std::string getTitle() { return m_Title; }
      inline GLFWwindow* getRawWindow() { return m_RawWindow; }
      inline GraphicsManager* getGraphicsManager() { return m_GraphicsManager; }
      inline Camera& getCamera() { return m_Camera; }

      static Window* currentWindow;

    protected:
      void createWindow();

      uint32_t m_Width;
      uint32_t m_Height;
      std::string m_Title;
      GLFWwindow* m_RawWindow = nullptr;
      GraphicsManager* m_GraphicsManager;
      Shader m_Shader;
      Camera m_Camera;
      glm::vec2 lastMousePos;
  };
}
