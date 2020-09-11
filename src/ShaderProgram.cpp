#include "ShaderProgram.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

ShaderProgram::ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader)
{
	m_ID = glCreateProgram();

	glAttachShader(m_ID, vertexShader.GetID());
	glAttachShader(m_ID, fragmentShader.GetID());

	glLinkProgram(m_ID);

	CheckLinkageStatus();
}

void ShaderProgram::Destroy()
{
	End();
	glDeleteProgram(m_ID);
}

ShaderProgram::~ShaderProgram()
{
	
}

void ShaderProgram::Use()
{
	glUseProgram(m_ID);
}

void ShaderProgram::End()
{
	glUseProgram(0);
}

void ShaderProgram::SetUniform(const std::string& name, GLboolean value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void ShaderProgram::SetUniform(const std::string& name, GLint value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void ShaderProgram::SetUniform(const std::string& name, GLuint value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void ShaderProgram::SetUniform(const std::string& name, GLfloat value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void ShaderProgram::SetUniform(const std::string& name, glm::vec2 value)
{
	glUniform2f(GetUniformLocation(name), value.x, value.y);
}

void ShaderProgram::SetUniform(const std::string& name, glm::vec3 value)
{
	glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void ShaderProgram::SetUniform(const std::string& name, glm::vec4 value)
{
	glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void ShaderProgram::SetUniform(const std::string& name, glm::mat2 value)
{
	glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::SetUniform(const std::string& name, glm::mat3 value)
{
	glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::SetUniform(const std::string& name, glm::mat4 value)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::CheckLinkageStatus()
{
	GLint success;
	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);

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

GLuint ShaderProgram::GetUniformLocation(const std::string& name)
{
	GLint location = glGetUniformLocation(m_ID, name.c_str());

	if (location == -1)
	{
		std::cerr << "Unable to find uniform " << name << ". It is either not declared or unused.\n";
	}

	return location;
}
