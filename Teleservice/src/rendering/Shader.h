#ifndef SHADER_HEADER
#define SHADER_HEADER

#include "vendor/glad/glad.h"
#include "ShaderType.h"
#include <string>
#include <vector>
#include "math/Matrix4x4.h"

class Shader
{
public:
	Shader();

	void AttachShader(const ShaderType& type, const std::string& path);
	void Create();
	inline void Use() const { glUseProgram(m_ID); }

	void SetUniformMat4(const std::string& name, const Matrix4x4& matrix) const;

private:
	unsigned int m_ID;
	unsigned int m_Shaders[5];
};
#endif