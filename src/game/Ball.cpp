#include "Ball.h"

Ball::Ball(const glm::vec2& position, float radius, const glm::vec3& color, Texture* sprite,
	const glm::vec2& velocity, const glm::vec4& boundaries)
	: GameObject(position, glm::vec2(2 * radius), color, sprite)
	, m_velocity(velocity)
	, m_radius(radius)
	, m_isStuck(true)
	, m_isSticky(false)
	, m_isPassingThrough(false)
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

bool Ball::IsStuck() const
{
	return m_isStuck;
}

const glm::vec4& Ball::GetBoundaries() const
{
	return m_boundaries;
}

bool Ball::IsSticky() const
{
	return m_isSticky;
}

bool Ball::IsPassingThrough() const
{
	return m_isPassingThrough;
}

void Ball::SetIsStuck(bool flag)
{
	m_isStuck = flag;
}

void Ball::SetIsSticky(bool flag)
{
	m_isSticky = flag;
}

void Ball::SetIsPassingThrough(bool flag)
{
	m_isPassingThrough = flag;
}

void Ball::SetVelocity(const glm::vec2& velocity)
{
	m_velocity = velocity;
}

void Ball::SetVelocity(float scale)
{
	m_velocity *= scale;
}

void Ball::SetVelocityX(float x)
{
	m_velocity.x = x;
}

void Ball::SetVelocityY(float y)
{
	m_velocity.y = y;
}
