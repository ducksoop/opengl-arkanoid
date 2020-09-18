#pragma once

#include "GameObject.h"

#include <glm/vec4.hpp>

class Ball : public GameObject
{
public:
	Ball(const glm::vec2& position,
	     float radius,
	     const glm::vec3& color,
	     Texture* sprite,
	     const glm::vec2& velocity,
	     const glm::vec4& boundaries);
	~Ball() = default;

	void Update(float dt);

	const glm::vec2& GetVelocity() const;
	float GetRadius() const;
	bool IsStuck() const;
	const glm::vec4& GetBoundaries() const;
	bool IsSticky() const;
	bool IsPassingThrough() const;

	void SetVelocity(const glm::vec2& velocity);
	void SetVelocity(float scale);
	void SetVelocityX(float x);
	void SetVelocityY(float y);
	void SetIsStuck(bool flag);
	void SetIsSticky(bool flag);
	void SetIsPassingThrough(bool flag);

private:
	glm::vec2 m_velocity;
	float m_radius;
	bool m_isStuck;
	bool m_isSticky;
	bool m_isPassingThrough;

	glm::vec4 m_boundaries;
};