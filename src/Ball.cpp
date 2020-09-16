#include "Ball.h"

#include <iostream>

Ball::Ball(const glm::vec2& position, float radius, const glm::vec3& color, std::shared_ptr<Texture> sprite,
	const glm::vec2& velocity, const glm::vec4& boundaries)
	: GameObject(position, glm::vec2(2 * radius), color, sprite)
	, m_velocity(velocity)
	, m_radius(radius)
	, m_isStuck(true)
	, m_boundaries(boundaries)
{
}

void Ball::Update(float dt)
{
	if (m_isStuck) return;

	m_position += m_velocity * dt;

	if (m_position.x <= m_boundaries.x)
	{
		m_velocity.x = -m_velocity.x;
		m_position.x = m_boundaries.x;
	}
	else if (m_position.x + m_size.x >= m_boundaries.y)
	{
		m_velocity.x = -m_velocity.x;
		m_position.x = m_boundaries.y - m_size.x;
	}
	else if (m_position.y <= m_boundaries.z)
	{
		m_velocity.y = -m_velocity.y;
		m_position.y = m_boundaries.z;
	}
	else if (m_position.y + m_size.y >= m_boundaries.w)
	{
		m_velocity.y = -m_velocity.y;
		m_position.y = m_boundaries.w - m_size.y;
	}
}

const glm::vec2& Ball::GetVelocity() const
{
	return m_velocity;
}

float Ball::GetRadius() const
{
	return m_radius;
}

bool Ball::GetIsStuck() const
{
	return m_isStuck;
}

void Ball::SetIsStuck(bool flag)
{
	m_isStuck = flag;
}
