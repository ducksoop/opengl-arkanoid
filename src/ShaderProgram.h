#pragma once

#include "Shader.h"

#include <GL/glew.h>

#include <string>

class ShaderProgram
{
public:
	ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);
	~ShaderProgram();

	void Use();
	void End();
	
private:
	void CheckLinkageStatus();
	GLuint GetUniformLocation(const std::string& name);
	
	GLuint m_ID;
};