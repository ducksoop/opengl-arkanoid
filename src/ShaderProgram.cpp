#include "ShaderProgram.h"

#include <iostream>

ShaderProgram::ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader)
{
	m_ID = glCreateProgram();

	glAttachShader(m_ID, vertexShader.GetID());
	glAttachShader(m_ID, fragmentShader.GetID());

	glLinkProgram(m_ID);

	CheckLinkageStatus();
}

ShaderProgram::~ShaderProgram()
{
	End();
	glDeleteProgram(m_ID);
}

void ShaderProgram::Use()
{
	glUseProgram(m_ID);
}

void ShaderProgram::End()
{
	glUseProgram(0);
}

void ShaderProgram::CheckLinkageStatus()
{
	GLint success;
	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);

	if (!success)
	{
		const int MAX_BUFFER_SIZE = 256;
		GLchar infoLog[MAX_BUFFER_SIZE];

		glGetProgramInfoLog(m_ID, MAX_BUFFER_SIZE, nullptr, infoLog);
		std::cerr << infoLog << std::endl;
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
