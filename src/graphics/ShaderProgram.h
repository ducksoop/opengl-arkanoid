#pragma once

#include "Shader.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>

class ShaderProgram
{
public:
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
	void SetUniform(const std::string& name, GLsizei count, GLboolean* values);
	void SetUniform(const std::string& name, GLsizei count, GLint* values);
	void SetUniform(const std::string& name, GLsizei count, GLuint* values);
	void SetUniform(const std::string& name, GLsizei count, GLfloat* values);
	void SetUniform(const std::string& name, GLsizei count, glm::vec2* values);
	void SetUniform(const std::string& name, GLsizei count, glm::vec3* values);
	void SetUniform(const std::string& name, GLsizei count, glm::vec4* values);
	void SetUniform(const std::string& name, GLsizei count, glm::mat2* values);
	void SetUniform(const std::string& name, GLsizei count, glm::mat3* values);
	void SetUniform(const std::string& name, GLsizei count, glm::mat4* values);
	
private:
	ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);

	void Destroy();
	void CheckLinkageStatus();
	GLuint GetUniformLocation(const std::string& name);
	
	GLuint m_ID;

	friend class ResourceManager;
};