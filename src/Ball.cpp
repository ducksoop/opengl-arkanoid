#include "Ball.h"

#include <iostream>

Ball::Ball(const glm::vec2& position, float radius, const glm::vec3& color, std::shared_ptr<Texture> sprite,
           const glm::vec2& velocity)
	: GameObject(position, glm::vec2(2 * radius), color, sprite)
	, m_velocity(velocity)
	, m_radius(radius)
	, m_isStuck(true)
{
}

void Ball::Move(const glm::vec4& boundaries, float delta)
{
	if (m_isStuck) return;

	m_position += m_velocity * delta;

	if (m_position.x <= boundaries.x)
	{
		m_velocity.x = -m_velocity.x;
		m_position.x = boundaries.x;
	}
	else if (m_position.x + m_size.x >= boundaries.y)
	{
		m_velocity.x = -m_velocity.x;
		m_position.x = boundaries.y - m_size.x;
	}
	else if (m_position.y <= boundaries.z)
	{
		m_velocity.y = -m_velocity.y;
		m_position.y = boundaries.z;
	}
	else if (m_position.y + m_size.y >= boundaries.w)
	{
		m_velocity.y = -m_velocity.y;
		m_position.y = boundaries.w - m_size.y;
	}
}
