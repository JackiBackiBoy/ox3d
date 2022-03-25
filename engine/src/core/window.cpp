#include "window.h"
#include <iostream>
#include <vector>
#include "rendering/model.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "ui/uiFont.h"

using namespace ox;

Window::Window(const uint32_t& width, const uint32_t& height, const std::string& title)
  : m_Width(width), m_Height(height), m_Title(title) {
    createWindow();
    m_GraphicsManager = new GraphicsManager();
}

void Window::createWindow() {
  glfwInit(); // initialize glfw

  // set GLFW options
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  m_RawWindow = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(m_RawWindow, &m_GraphicsManager);
  glfwSetFramebufferSizeCallback(m_RawWindow, m_GraphicsManager->onResize);
  glfwSetInputMode(m_RawWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSwapInterval(1);
  glfwSetCursorPosCallback(m_RawWindow, Mouse::onMouseMove);
  glfwSetScrollCallback(m_RawWindow, Mouse::onMouseScroll);
}

void Window::onStart() {
  // Shaders
  m_Shader.loadVertexShader("assets/shaders/lightingShader.vert.spv");
  m_Shader.loadFragmentShader("assets/shaders/lightingShader.frag.spv");
  m_GraphicsManager->addShader(&m_Shader);

  // Models
  Model waterbottle;
  waterbottle.loadFromFile("assets/models/waterbottle/WaterBottle.gltf");
  m_GraphicsManager->submitModel(waterbottle);

  m_GraphicsManager->m_Vertices = waterbottle.getVertices();
  m_GraphicsManager->m_Indices = waterbottle.getIndices();

  // Camera
  m_Camera = Camera();
  m_Camera.setPosition({ 0.0f, 0.0f, 2.0f });
  m_Camera.setYaw(90.0f);
  m_Camera.setFOV(60.0f);

  // User Interface
  UIFont font;
  font.loadFromFile("assets/fonts/segoeui.ttf", 12);
}

void Window::onUpdate(const float& deltaTime) {
  // ------ Rotation with mouse ------
  glm::vec2 mousePos = Mouse::getPosition();

  // Fix for preventing violent camera rotation before
  // or during the initial movement with the mouse
  if (!Mouse::hasMoved || Mouse::firstMove) {
    lastMousePos = mousePos;
  }

  glm::vec2 deltaMousePos = mousePos - lastMousePos;

  m_Camera.setYaw(m_Camera.getYaw() + 0.05f * deltaMousePos.x);
  m_Camera.setPitch(m_Camera.getPitch() + 0.05f * deltaMousePos.y);

  glm::vec3 camPos = m_Camera.getPosition();
  glm::vec3 camRight = m_Camera.getRight();
  glm::vec3 camUp = m_Camera.getUp();
  glm::vec3 camForward = m_Camera.getForward();

  // ------ Mouse scrolling ------
  float scrollDir = Mouse::getVerticalScroll();

  if (scrollDir != 0.0f) {
    m_Camera.setFOV(m_Camera.getFOV() - scrollDir);
  }

  // ------ Keyboard movement ------
  // Move forwards
  if (Keyboard::isKeyDown(KeyCode::W)) {
    m_Camera.setPosition(camPos + camForward * deltaTime * 3.0f);
  }
  // Move horizontally to the left
  if (Keyboard::isKeyDown(KeyCode::A)) {
    m_Camera.setPosition(camPos - camRight * deltaTime * 3.0f);
  }
  // Move backwards
  if (Keyboard::isKeyDown(KeyCode::S)) {
    m_Camera.setPosition(camPos - camForward * deltaTime * 3.0f);
  }
  // Move horizontally to the right
  if (Keyboard::isKeyDown(KeyCode::D)) {
    m_Camera.setPosition(camPos + camRight * deltaTime * 3.0f);
  }

  // Move upwards
  if (Keyboard::isKeyDown(KeyCode::Space)) {
    m_Camera.setPosition({ camPos.x, camPos.y + deltaTime * 3.0f, camPos.z });
  }
  // Move downwards
  if (Keyboard::isKeyDown(KeyCode::LeftControl)) {
    m_Camera.setPosition({ camPos.x, camPos.y - deltaTime * 3.0f, camPos.z });
  }

  m_Camera.update();

  lastMousePos = mousePos;
}

void Window::onRender() {

}

void Window::run() {
  Window::onStart();
  onStart();

  m_GraphicsManager->loadVulkan();


  // Rendering loop
  float deltaTime = 0.0f;
  while (!glfwWindowShouldClose(m_RawWindow)) {
    float t0 = static_cast<float>(glfwGetTime());


    glfwPollEvents();
    m_GraphicsManager->renderFrame();

    Window::onUpdate(deltaTime);
    onUpdate(deltaTime);

    deltaTime = static_cast<float>(glfwGetTime()) - t0;
  }

  // onExit
  m_GraphicsManager->destroyVulkan();
  glfwDestroyWindow(m_RawWindow);
  glfwTerminate();
}

Window* Window::currentWindow = nullptr;
