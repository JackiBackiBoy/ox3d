#include "window.h"
#include <iostream>
#include <vector>
#include "graphicsManager.h"
#include "rendering/model.h"

Window::Window(const uint32_t& width, const uint32_t& height, const std::string& title)
  : m_Width(width), m_Height(height), m_Title(title) {
    createWindow();
    m_GraphicsManager = new GraphicsManager(this);
}

void Window::createWindow() {
  glfwInit(); // initialize glfw

  // set GLFW options
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  m_RawWindow = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(m_RawWindow, &m_GraphicsManager);
  glfwSetFramebufferSizeCallback(m_RawWindow, m_GraphicsManager->onResize);
}

void Window::onStart() {
  // Shaders
  m_Shader.loadVertexShader("shaders/lightingShader.vert.spv");
  m_Shader.loadFragmentShader("shaders/lightingShader.frag.spv");
  m_GraphicsManager->addShader(&m_Shader);

  // Models
  Model cube;
  cube.loadFromFile("assets/models/cube.gltf");
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

