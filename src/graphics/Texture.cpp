#include "Texture.h"

#include <GL/glew.h>
#include <stb_image.h>

#include <iostream>

Texture::Texture(GLuint w, GLuint h, unsigned char* image, GLuint format)
	: m_width(w)
	, m_height(h)
	, m_format(format)
{
	glGenTextures(1, &m_ID);

	Bind();

	glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, image);

	stbi_image_free(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	UnBind();
}

void Texture::Destroy() const
{
	glDeleteTextures(1, &m_ID);
}

Texture::~Texture()
{
}

void Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::Bind(GLuint uint) const
{
	glActiveTexture(GL_TEXTURE0 + uint);
	Bind();
}

void Texture::UnBind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
