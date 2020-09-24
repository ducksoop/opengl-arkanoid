#include "ParticleEmitter.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

ParticleEmitter::ParticleEmitter(ShaderProgram* shaderProgram, Texture* texture, GLuint amount)
	: m_amount(amount)
	, m_shaderProgram(shaderProgram)
	, m_texture(texture)
	, m_lastUnusedParticle(0)
{
	GLfloat quad[] =
	{
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	for (GLuint i = 0; i < m_amount; i++)
		m_particles.emplace_back();
}

ParticleEmitter::~ParticleEmitter()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}

void ParticleEmitter::Update(float delta, const Ball& object, int newParticles, glm::vec2 offset)
{
	// Add new particles
	for (GLuint i = 0; i < newParticles; i++)
	{
		int unusedParticle = GetFirstUnusedParticle();
		RespawnParticle(m_particles[unusedParticle], object, offset);
	}

	// Update all particles
	for (auto& particle : m_particles)
	{
		particle.lifespan -= delta;
		if (particle.lifespan > 0.0f)
		{
			particle.position -= particle.velocity * delta;
			particle.color.a -= delta * 2.5;
		}
	}
}

void ParticleEmitter::Render(float scale)
{
	// Utilize additive blending to get a "glow" effect
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	m_shaderProgram->Use();

	m_shaderProgram->SetUniform("scale", scale);
	
	for (auto& particle : m_particles)
	{
		if (particle.lifespan > 0.0f)
		{
			m_shaderProgram->SetUniform("offset", particle.position);
			m_shaderProgram->SetUniform("particleColor", particle.color);
			m_texture->Bind();
			glBindVertexArray(m_VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}

	m_shaderProgram->End();

	// Reset to default blending mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

GLuint ParticleEmitter::GetFirstUnusedParticle()
{
	// Search from last used particle
	for (auto i = m_lastUnusedParticle; i < m_amount; i++)
	{
		if (m_particles[i].lifespan <= 0.0f)
		{
			m_lastUnusedParticle = i;
			return i;
		}
	}

	// Else, do a linear search
	for (GLuint i = 0; i < m_lastUnusedParticle; i++)
	{
		if (m_particles[i].lifespan <= 0.0f)
		{
			m_lastUnusedParticle = i;
			return i;
		}
	}

	// All particles are taken, override the first one
	m_lastUnusedParticle = 0;
	return m_lastUnusedParticle;
}

void ParticleEmitter::RespawnParticle(Particle& particle, const Ball& object, const glm::vec2& offset)
{
	auto random = ((rand() % 100) - 50) / 10.0f;
	auto color = static_cast<GLfloat>(0.5) + ((rand() % 100) / 100.0f);
	particle.position = object.GetPosition() + random + offset;
	particle.color = glm::vec4(color, color, color, 1.0f);
	particle.lifespan = 1.0f;
	particle.velocity = object.GetVelocity();
}
