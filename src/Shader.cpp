#include "Shader.h"
#include "ShaderType.h"

#include <GL/glew.h>

#include <iostream>

Shader::Shader(ShaderType type, const std::string& source)
{
	m_ID = glCreateShader(type);
	
	const GLchar* shaderSource = source.c_str();
	glShaderSource(m_ID, 1, &shaderSource, nullptr);
	glCompileShader(m_ID);

	CheckCompilationStatus();
}

Shader::~Shader()
{
	glDeleteShader(m_ID);
}

GLuint Shader::GetID() const
{
	return m_ID;
}

void Shader::CheckCompilationStatus()
{
	GLint success;
	glGetShaderiv(m_ID, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		const int MAX_BUFFER_SIZE = 256;
		GLchar infoLog[MAX_BUFFER_SIZE];

		glGetShaderInfoLog(m_ID, MAX_BUFFER_SIZE, nullptr, infoLog);
		std::cerr << infoLog << std::endl;
	}
}
