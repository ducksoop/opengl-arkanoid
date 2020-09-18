#include "Powerup.h"

namespace {
	const glm::vec2 SIZE(60, 20);
	const glm::vec2 VELOCITY(0.0f, 150.0f);
}

Powerup::Powerup(const glm::vec2& position, const glm::vec3& color, Texture* sprite, PowerupType type, GLfloat duration)
	: GameObject(position, SIZE, color, sprite)
	, m_type(type)
	, m_duration(duration)
	, m_isActivated(false)
	, m_isDestroyed(false)
	, m_velocity(VELOCITY)
{
}

void Powerup::Update(float dt, Paddle& paddle, Ball& ball, PostProcessing& postProcessing)
{
	m_position += m_velocity * dt;
	if (!m_isActivated)
		return;

	m_duration -= dt;
	if (m_duration <= 0.0f)
	{
		m_isActivated = false;

		// Deactivate effects
		switch (m_type)
		{
		case PowerupType::Sticky:
			ball.SetIsSticky(false);
			paddle.SetColor(glm::vec3(1.0f));
			break;
		case PowerupType::PassThrough:
			ball.SetIsPassingThrough(false);
			ball.SetColor(glm::vec3(1.0f));
			break;
		case PowerupType::Confuse:
			postProcessing.DisableEffects(PostProcessingEffect::Confuse);
			break;
		case PowerupType::Chaos:
			postProcessing.DisableEffects(PostProcessingEffect::Chaos);
			break;
		}
	}
}

void Powerup::Activate(Paddle& paddle, Ball& ball, PostProcessing& postProcessing)
{
	m_isActivated = true;
	m_isDestroyed = true;

	switch (m_type)
	{
	case PowerupType::SpeedUp:
		ball.SetVelocity(1.2f);
		break;
	case PowerupType::Sticky:
		ball.SetIsSticky(true);
		paddle.SetColor(glm::vec3(1.0f, 0.5f, 1.0f));
		break;
	case PowerupType::PassThrough:
		ball.SetIsPassingThrough(true);
		ball.SetColor(glm::vec3(1.0f, 0.5f, 0.5f));
		break;
	case PowerupType::PaddleSizeIncrease:
		paddle.UpdateSizeX(50);
		break;
	case PowerupType::Confuse:
		postProcessing.EnableEffects(PostProcessingEffect::Confuse);
		break;
	case PowerupType::Chaos:
		postProcessing.EnableEffects(PostProcessingEffect::Chaos);
		break;
	}
}

PowerupType Powerup::GetType() const
{
	return m_type;
}

GLfloat Powerup::GetDuration() const
{
	return m_duration;
}

bool Powerup::IsActivated() const
{
	return m_isActivated;
}

bool Powerup::IsDestroyed() const
{
	return m_isDestroyed;
}

const glm::vec2& Powerup::GetVelocity() const
{
	return m_velocity;
}

void Powerup::SetIsDestroyed(bool flag)
{
	m_isDestroyed = flag;
}
