#pragma once

#include <GL/glew.h>

class Texture
{
public:
	Texture(GLuint w, GLuint h, unsigned char* image, GLuint format = GL_RGB);
	~Texture();

	void Bind();
	void Bind(GLuint uint);
	void UnBind();
	
private:
	GLuint m_ID;
	GLuint m_width;
	GLuint m_height;
	GLuint m_format;
};
