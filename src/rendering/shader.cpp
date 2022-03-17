#include "shader.h"
#include <fstream>
#include <iostream>

using namespace ox;

void Shader::loadVertexShader(const std::string& path) {
  std::string enginePath = ENGINE_DIR + path;
  std::ifstream fileStream(enginePath, std::ios::ate | std::ios::binary); // load binary SPIRV file (from the end)

  if (!fileStream.is_open()) {
    throw std::runtime_error("SHADER ERROR: Failed to open shader file!");
  }

  size_t fileSize = (size_t)fileStream.tellg();
  std::vector<char> buffer(fileSize);
  fileStream.seekg(0);
  fileStream.read(buffer.data(), fileSize);
  fileStream.close();

  m_VertexShaderCode = buffer;
}

void Shader::loadFragmentShader(const std::string& path) {
  std::string enginePath = ENGINE_DIR + path;
  std::ifstream fileStream(enginePath, std::ios::ate | std::ios::binary); // load binary SPIRV file (from the end)

  if (!fileStream.is_open()) {
    throw std::runtime_error("SHADER ERROR: Failed to open shader file!");
  }

  size_t fileSize = (size_t)fileStream.tellg();
  std::vector<char> buffer(fileSize);
  fileStream.seekg(0);
  fileStream.read(buffer.data(), fileSize);
  fileStream.close();

  m_FragmentShaderCode = buffer;
}
