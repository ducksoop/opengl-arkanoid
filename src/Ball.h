#pragma once

#include "GameObject.h"

#include <glm/vec4.hpp>

class Ball : public GameObject
{
public:
	Ball(const glm::vec2& position,
	     float radius,
	     const glm::vec3& color,
	     std::shared_ptr<Texture> sprite,
	     const glm::vec2& velocity,
	     const glm::vec4& boundaries);
	~Ball() = default;

	void Update(float dt);

	const glm::vec2& GetVelocity() const;
	float GetRadius() const;
	bool IsStuck() const;
	const glm::vec4& GetBoundaries() const;

	void SetVelocity(const glm::vec2 velocity);
	void SetVelocityX(float x);
	void SetVelocityY(float y);
	void SetIsStuck(bool flag);

private:
	glm::vec2 m_velocity;
	float m_radius;
	bool m_isStuck;

	glm::vec4 m_boundaries;
};