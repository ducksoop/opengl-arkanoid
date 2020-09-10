#pragma once

#include "Shader.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>

class ShaderProgram
{
public:
	ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);
	~ShaderProgram();

	void Use();
	void End();

	void SetUniform(const std::string& name, GLboolean value);
	void SetUniform(const std::string& name, GLint value);
	void SetUniform(const std::string& name, GLuint value);
	void SetUniform(const std::string& name, GLfloat value);
	void SetUniform(const std::string& name, glm::vec2 value);
	void SetUniform(const std::string& name, glm::vec3 value);
	void SetUniform(const std::string& name, glm::vec4 value);
	void SetUniform(const std::string& name, glm::mat2 value);
	void SetUniform(const std::string& name, glm::mat3 value);
	void SetUniform(const std::string& name, glm::mat4 value);
	
private:
	void CheckLinkageStatus();
	GLuint GetUniformLocation(const std::string& name);
	
	GLuint m_ID;
};