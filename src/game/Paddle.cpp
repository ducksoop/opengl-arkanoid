#include "Paddle.h"

Paddle::Paddle(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color, Texture* sprite, float velocity, const glm::vec2& boundaries)
	: GameObject(position, size, color, sprite)
	, m_velocity(velocity)
	, m_boundaries(boundaries)
{
}

float Paddle::GetVelocity() const
{
	return m_velocity;
}

const glm::vec2& Paddle::GetBoundaries() const
{
	return m_boundaries;
}

void Paddle::SetBoundaries(const glm::vec2& boundaries)
{
	m_boundaries = boundaries;
}
