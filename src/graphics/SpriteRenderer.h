#pragma once

#include "Singleton.h"
#include "ShaderProgram.h"
#include "Texture.h"

#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <memory>

class SpriteRenderer : public Singleton<SpriteRenderer>
{
public:
	void Initialize(ShaderProgram* shaderProgram);
	void SetShaderProgram(ShaderProgram* shaderProgram);
	void RenderSprite(const Texture* texture,
	                  const glm::vec2& position,
	                  const glm::vec2& size = glm::vec2(10, 10),
	                  const glm::vec3& color = glm::vec3(1.0f),
	                  GLfloat rotate = 0.0f) const;
	
private:
	SpriteRenderer();
	~SpriteRenderer();

	ShaderProgram* m_shaderProgram;
	GLuint m_VAO;
	GLuint m_VBO;

	friend Singleton;
};
