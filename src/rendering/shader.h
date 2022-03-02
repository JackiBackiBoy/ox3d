#ifndef SHADER_HEADER
#define SHADER_HEADER

#include <string>
#include <vector>

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
#endif
