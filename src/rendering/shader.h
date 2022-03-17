#pragma once

#include <string>
#include <vector>

namespace ox {
  class Shader {
    public:
      Shader() {};

      void loadVertexShader(const std::string& path);
      void loadFragmentShader(const std::string& path);

      inline std::vector<char>& getVertexShaderCode() { return m_VertexShaderCode; }
      inline std::vector<char>& getFragmentShaderCode() { return m_FragmentShaderCode; }

    private:
      std::vector<char> m_VertexShaderCode;
      std::vector<char> m_FragmentShaderCode;
  };
}
