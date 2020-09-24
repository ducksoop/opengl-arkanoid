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

void ShaderProgram::Use() const
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

void ShaderProgram::SetUniform(const std::string& name, const glm::vec2& value)
{
	glUniform2f(GetUniformLocation(name), value.x, value.y);
}

void ShaderProgram::SetUniform(const std::string& name, const glm::vec3& value)
{
	glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void ShaderProgram::SetUniform(const std::string& name, const glm::vec4& value)
{
	glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void ShaderProgram::SetUniform(const std::string& name, const glm::mat2& value)
{
	glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::SetUniform(const std::string& name, const glm::mat3& value)
{
	glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::SetUniform(const std::string& name, const glm::mat4& value)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}


void ShaderProgram::SetUniform(const std::string& name, GLsizei count, GLboolean* values)
{
	glUniform1iv(GetUniformLocation(name), count, reinterpret_cast<GLint*>(values));
}

void ShaderProgram::SetUniform(const std::string& name, GLsizei count, GLint* values)
{
	glUniform1iv(GetUniformLocation(name), count, values);
}

void ShaderProgram::SetUniform(const std::string& name, GLsizei count, GLuint* values)
{
	glUniform1iv(GetUniformLocation(name), count, reinterpret_cast<GLint*>(values));
}

void ShaderProgram::SetUniform(const std::string& name, GLsizei count, GLfloat* values)
{
	glUniform1fv(GetUniformLocation(name), count, values);
}

void ShaderProgram::SetUniform(const std::string& name, GLsizei count, const glm::vec2* values)
{
	glUniform2fv(GetUniformLocation(name), count, &values[0].x);
}

void ShaderProgram::SetUniform(const std::string& name, GLsizei count, const glm::vec3* values)
{
	glUniform3fv(GetUniformLocation(name), count, &values[0].x);
}

void ShaderProgram::SetUniform(const std::string& name, GLsizei count, const glm::vec4* values)
{
	glUniform4fv(GetUniformLocation(name), count, &values[0].x);
}

void ShaderProgram::SetUniform(const std::string& name, GLsizei count, const glm::mat2* values)
{
	glUniformMatrix2fv(GetUniformLocation(name), count, GL_FALSE, &values[0][0].x);
}

void ShaderProgram::SetUniform(const std::string& name, GLsizei count, const glm::mat3* values)
{
	glUniformMatrix3fv(GetUniformLocation(name), count, GL_FALSE, &values[0][0].x);
}

void ShaderProgram::SetUniform(const std::string& name, GLsizei count, const glm::mat4* values)
{
	glUniformMatrix4fv(GetUniformLocation(name), count, GL_FALSE, &values[0][0].x);
}

void ShaderProgram::CheckLinkageStatus() const
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

GLuint ShaderProgram::GetUniformLocation(const std::string& name) const
{
	auto location = glGetUniformLocation(m_ID, name.c_str());

	if (location == -1)
	{
		std::cerr << "Unable to find uniform " << name << ". It is either not declared or unused.\n";
	}

	return location;
}
