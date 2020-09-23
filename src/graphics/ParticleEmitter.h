#pragma once

#include "Texture.h"
#include "ShaderProgram.h"
#include "Ball.h"
#include "Particle.h"

#include <GL/glew.h>

#include <vector>

class ParticleEmitter
{
public:
	ParticleEmitter(ShaderProgram* shaderProgram, Texture* texture, GLuint amount);
	~ParticleEmitter();

	void Update(float delta,
	            const Ball& object,
	            int newParticles,
	            glm::vec2 offset = glm::vec2(0.0f));
	void Render(float scale);
	
private:
	GLuint GetFirstUnusedParticle();
	void RespawnParticle(Particle& particle,
	                     const Ball& object,
	                     const glm::vec2& offset = glm::vec2(0.0f));
	
	std::vector<Particle> m_particles;
	GLuint m_amount;
	ShaderProgram* m_shaderProgram;
	Texture* m_texture;

	GLuint m_VAO;
	GLuint m_VBO;

	GLuint m_lastUnusedParticle;
};
