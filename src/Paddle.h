#pragma once

#include "GameObject.h"
#include "InputManager.h"

#include <glm/vec2.hpp>

#include <memory>

class Paddle : public GameObject
{
public:
	Paddle(const glm::vec2& position,
	       const glm::vec2& size,
	       const glm::vec3& color,
	       std::shared_ptr<Texture> sprite,
	       float velocity,
	       const glm::vec2& boundaries);
    ~Paddle();
	
private:
	float m_velocity;
	glm::vec2 m_boundaries;

	InputManager& m_inputManager = InputManager::Instance();
};
