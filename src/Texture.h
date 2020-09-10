#pragma once

#include <GL/glew.h>

class Texture
{
public:
	~Texture();

	void Bind();
	void Bind(GLuint uint);
	void UnBind();
	
private:
	Texture(GLuint w, GLuint h, unsigned char* image, GLuint format = GL_RGB);

	void Destroy();

	GLuint m_ID;
	GLuint m_width;
	GLuint m_height;
	GLuint m_format;

	friend class ResourceManager;
};
