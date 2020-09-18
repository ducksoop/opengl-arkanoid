#pragma once

#include <GL/glew.h>

#include "ShaderProgram.h"
#include "Texture.h"
#include "PostProcessingEffect.h"

class PostProcessing
{
public:
	PostProcessing(ShaderProgram* shaderProgram, GLuint w, GLuint h);
	~PostProcessing();

	// Prepares the framebuffer operations before rendering the game
	void BeginRender();

	// Should be called after rendering the game, sores all the rendered data into a texture object
	void EndRender();

	// Renders the texture quad
	void Render(GLfloat delta);

	void EnableEffects(int effects);
	void DisableEffects(int effects);
	
private:
	void InitializeFBOs();
	void InitializeTexture();
	void InitializeVAO();
	void InitializeShaderUniforms();
	
	ShaderProgram* m_shaderProgram;
	Texture* m_texture;

	GLuint m_width;
	GLuint m_height;

	int m_effects;

	GLuint m_MSFBO;		// Multi-sampled Framebuffer Object
	GLuint m_FBO;		// Framebuffer Object
	GLuint m_RBO;		// Renderbuffer Object
	GLuint m_VAO;		// Vertex Array Object
	GLuint m_VBO;		// Vertex Buffer Object
	GLuint m_textureID;
};