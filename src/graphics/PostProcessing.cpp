#include "PostProcessing.h"

#include <glm/glm.hpp>

#include <stdexcept>
#include <iostream>

PostProcessing::PostProcessing(ShaderProgram* shaderProgram, GLuint w, GLuint h)
	: m_shaderProgram(shaderProgram)
	, m_width(w)
	, m_height(h)
	, m_effects(0)
{
	InitializeFBOs();
	InitializeVAO();
	InitializeShaderUniforms();
}

PostProcessing::~PostProcessing()
{
	glDeleteFramebuffers(1, &m_MSFBO);
	glDeleteFramebuffers(1, &m_FBO);
	glDeleteFramebuffers(1, &m_RBO);
	glDeleteFramebuffers(1, &m_textureID);
	glDeleteFramebuffers(1, &m_VAO);
	glDeleteFramebuffers(1, &m_VBO);
}

void PostProcessing::BeginRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_MSFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessing::EndRender()
{
	// Resolve multi-sampled color-buffer into intermediate FBO to store texture
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MSFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
	glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	// Bind both READ and WRITE framebuffer to default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessing::Render(GLfloat delta)
{
	m_shaderProgram->Use();
	m_shaderProgram->SetUniform("time", delta);
	m_shaderProgram->SetUniform("chaos", m_effects & PostProcessingEffect::Chaos);
	m_shaderProgram->SetUniform("confuse", m_effects & PostProcessingEffect::Confuse);
	m_shaderProgram->SetUniform("shake", m_effects & PostProcessingEffect::Shake);

	// Render textured quad
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void PostProcessing::EnableEffects(int effects)
{
	if (effects & PostProcessingEffect::Chaos) m_effects |= PostProcessingEffect::Chaos;
	if (effects & PostProcessingEffect::Confuse) m_effects |= PostProcessingEffect::Confuse;
	if (effects & PostProcessingEffect::Shake) m_effects |= PostProcessingEffect::Shake;
}

void PostProcessing::DisableEffects(int effects)
{
	if (effects & PostProcessingEffect::Chaos) m_effects &= ~PostProcessingEffect::Chaos;
	if (effects & PostProcessingEffect::Confuse) m_effects &= ~PostProcessingEffect::Confuse;
	if (effects & PostProcessingEffect::Shake) m_effects &= ~PostProcessingEffect::Shake;
}

void PostProcessing::InitializeFBOs()
{
	// Initialize framebuffer/renderbuffer objects
	glGenFramebuffers(1, &m_MSFBO);
	glGenFramebuffers(1, &m_FBO);
	glGenRenderbuffers(1, &m_RBO);

	// Initialize renderbuffer storage with a multi-sampled color buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_MSFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);

	// Allocate storage for renderbuffer object
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, m_width, m_height);

	// Attach multi-sample renderbuffer object to framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Failed to initialize MSFBO.\n";
		throw std::runtime_error("Failed to initialize MSFBO.");
	}

	// Initialize the FBO/texture to blit multi-sampled color-buffer to use for shader operations
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	InitializeTexture();

	// Attach texture to framebuffer as its color attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureID, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Failed to initialize FBO.\n";
		throw std::runtime_error("Failed to initialize FBO.");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessing::InitializeTexture()
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void PostProcessing::InitializeVAO()
{
	GLfloat quad[] =
	{
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 1.0f,

		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void PostProcessing::InitializeShaderUniforms()
{
	m_shaderProgram->Use();
	m_shaderProgram->SetUniform("scene", 0);

	GLfloat offset = 1.0f / 300.0f;
	glm::vec2 offsets[] =
	{
		{ -offset, offset },	// Top-left
		{ 0.0f, offset },		// Top-center
		{ offset, offset },	// Top-right
		{ -offset, 0.0f },		// Center-left
		{ 0.0f, 0.0f },		// Center-center
		{ offset, 0.0f },		// Center-right
		{ -offset, -offset },	// Bottom-left
		{ 0.0f, -offset },		// Bottom-center
		{ offset, -offset },	// Bottom-right
	};

	m_shaderProgram->SetUniform("offsets", 9, offsets);

	GLint edgeKernel[] =
	{
		-1, -1, -1,
		-1, -8, -1,
		-1, -8, -1
	};

	m_shaderProgram->SetUniform("edgeKernel", 9, edgeKernel);

	GLfloat blurKernel[9] =
	{
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
		2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
	};

	m_shaderProgram->SetUniform("blueKernel", 9, blurKernel);

	m_shaderProgram->End();
}
