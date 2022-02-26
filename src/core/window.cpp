#include "window.h"
#include <iostream>
#include <vector>
#include "graphicsManager.h"

Window::Window(const uint32_t& width, const uint32_t& height, const std::string& title)
  : m_Width(width), m_Height(height), m_Title(title) {
    createWindow();
    m_GraphicsManager = new GraphicsManager(this);
}

void Window::createWindow() {
  glfwInit(); // initialize glfw

  // set GLFW options
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  m_RawWindow = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
}

void Window::onStart() {
  m_Shader.loadVertexShader("assets/shaders/lightingShaderVert.spv");
  m_Shader.loadFragmentShader("assets/shaders/lightingShaderFrag.spv");

  m_GraphicsManager->addShader(&m_Shader);
}

void Window::onUpdate() {

}

void Window::onRender() {

}

void Window::run() {
  onStart();

  m_GraphicsManager->loadVulkan();


  // Rendering loop
  while (!glfwWindowShouldClose(m_RawWindow)) {
    glfwPollEvents();
    m_GraphicsManager->renderFrame();
  }

  // onExit
  m_GraphicsManager->destroyVulkan();
  glfwDestroyWindow(m_RawWindow);
  glfwTerminate();
}

