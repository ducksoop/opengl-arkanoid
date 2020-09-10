#include "Shader.h"
#include "ShaderType.h"
#include "FileManager.h"

#include <GL/glew.h>

#include <iostream>
#include <stdexcept>
#include <vector>

Shader::Shader(ShaderType type, const std::string& path)
{
	const std::string source = FileManager::Instance().ReadAsText(path);
	
	m_ID = glCreateShader(static_cast<GLenum>(type));
	
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
		GLint LOG_LENGTH;
		glGetShaderiv(m_ID, GL_INFO_LOG_LENGTH, &LOG_LENGTH);

		std::vector<GLchar> errorLog(static_cast<GLuint>(LOG_LENGTH));

		glGetShaderInfoLog(m_ID, LOG_LENGTH, nullptr, &errorLog[0]);
		std::cerr << &errorLog[0] << std::endl;
		throw std::runtime_error("Shader compilation failed");
	}
}
