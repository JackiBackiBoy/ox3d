#include "Shader.h"
#include <fstream>
#include <sstream>

Shader::Shader()
{

}

void Shader::AttachShader(const ShaderType& type, const std::string& path)
{
	unsigned int glShaderType = 0;

	switch (type)
	{
		case ShaderType::Vertex: glShaderType = GL_VERTEX_SHADER;
			break;
		case ShaderType::Fragment: glShaderType = GL_FRAGMENT_SHADER;
			break;
		case ShaderType::Geometry: glShaderType = GL_GEOMETRY_SHADER;
			break;
		case ShaderType::Compute: glShaderType = GL_COMPUTE_SHADER;
			break;
	}

	std::string shaderCode;
	std::ifstream shaderFile;

	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		shaderFile.open(path);

		std::stringstream shaderStringStream;
		shaderStringStream << shaderFile.rdbuf();

		shaderFile.close();
		shaderCode = shaderStringStream.str();
	}
	catch (std::ifstream::failure f)
	{
		//std::cout << "Shader error: file could not be successfully loaded!" << std::endl;
	}

	const char* rawShaderCode = shaderCode.c_str();

	// ----- Compile shaders -----
	int tempSuccess;
	char tempInfoLog[512];

	// Compile shader
	m_Shaders[(unsigned int)type] = glCreateShader(glShaderType);
	glShaderSource(m_Shaders[(unsigned int)type], 1, &rawShaderCode, nullptr);
	glCompileShader(m_Shaders[(unsigned int)type]);
	glGetShaderiv(m_Shaders[(unsigned int)type], GL_COMPILE_STATUS, &tempSuccess);

	if (!tempSuccess)
	{
		glGetShaderInfoLog(m_Shaders[(unsigned int)type], 512, nullptr, tempInfoLog);
		//std::cout << "OpenGL error: shader compilation failed! " << tempInfoLog << std::endl;
	}
}

void Shader::Create()
{
	int i = m_Shaders[0];

	// ----- Create shader program -----
	m_ID = glCreateProgram(); // final linked version of multiple shaders combined
	glAttachShader(m_ID, m_Shaders[0]); // attach vertex shader
	glAttachShader(m_ID, m_Shaders[1]); // attach fragment shader
	glLinkProgram(m_ID);

	// Check if linking of shader program was successful
	int tempSuccess;
	char tempInfoLog[512];

	glGetProgramiv(m_ID, GL_LINK_STATUS, &tempSuccess);

	if (!tempSuccess)
	{
		glGetProgramInfoLog(m_ID, 512, nullptr, tempInfoLog);
		//std::cout << "OpenGL error: shader program linking failed! " << tempInfoLog << std::endl;
	}

	// Once linked, delete the shader objects
	glDeleteShader(m_Shaders[0]);
	glDeleteShader(m_Shaders[1]);
}

void Shader::SetUniformMat4(const std::string& name, const Matrix4x4& matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, false, &(matrix.data[0][0]));
}
