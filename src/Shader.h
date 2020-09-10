#pragma once

#include "ShaderType.h"

#include <GL/glew.h>

class Shader
{
public:
	Shader(ShaderType type, const GLchar* source);
	~Shader();

	GLuint GetID() const;
	
private:
	void CheckCompilationStatus();
	
	GLuint m_ID;
};