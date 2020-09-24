#pragma once

#include <GL/glew.h>

class Texture
{
public:
	~Texture();

	void Bind() const;
	void Bind(GLuint uint) const;
	void UnBind() const;
	
private:
	Texture(GLuint w, GLuint h, unsigned char* image, GLuint format);

	void Destroy() const;

	GLuint m_ID;
	GLuint m_width;
	GLuint m_height;
	GLuint m_format;

	friend class ResourceManager;
};
