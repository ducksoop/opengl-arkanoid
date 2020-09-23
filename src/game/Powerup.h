#pragma once

#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "GameObject.h"
#include "Ball.h"
#include "Paddle.h"
#include "PostProcessing.h"
#include "PowerupType.h"

class Powerup : public GameObject
{
public:
	Powerup(const glm::vec2& position,
			const glm::vec2& size,
	        const glm::vec3& color,
	        Texture* sprite,
			const glm::vec2& velocity,
	        PowerupType type,
	        GLfloat duration);
	~Powerup() = default;

	void Update(float dt, Paddle& paddle, Ball& ball, PostProcessing& postProcessing);
	void Activate(Paddle& paddle, Ball& ball, PostProcessing& postProcessing, const glm::vec2 scales);

	PowerupType GetType() const;
	GLfloat GetDuration() const;
	bool IsActivated() const;
	bool IsDestroyed() const;
	const glm::vec2& GetVelocity() const;

	void SetIsDestroyed(bool flag);
	
private:
	PowerupType m_type;
	GLfloat m_duration;
	bool m_isActivated;
	bool m_isDestroyed;
	glm::vec2 m_velocity;
};
