#pragma once

#define GLFW_INCLUDE_VULKAN

#include <cstdint>
#include <string>
#include <GLFW/glfw3.h>
#include "rendering/shader.h"
#include "graphicsManager.h"

class Window {
  public:
    Window(const uint32_t& width, const uint32_t& height, const std::string& title);

    virtual void onStart();
    virtual void onUpdate();
    virtual void onRender();
    void run();

    inline std::string getTitle() { return m_Title; }
    inline GLFWwindow* getRawWindow() { return m_RawWindow; }
    inline GraphicsManager* getGraphicsManager() { return m_GraphicsManager; }

    static Window* currentWindow;

  private:
    void createWindow();

    uint32_t m_Width;
    uint32_t m_Height;
    std::string m_Title;
    GLFWwindow* m_RawWindow = nullptr;
    GraphicsManager* m_GraphicsManager;
    Shader m_Shader;
};
