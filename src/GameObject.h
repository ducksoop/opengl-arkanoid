#pragma once

#include "Texture.h"
#include "SpriteRenderer.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <memory>

class GameObject
{
public:
    GameObject(const glm::vec2& position,
        const glm::vec2& size,
        const glm::vec3& color,
        std::shared_ptr<Texture> sprite);
	virtual ~GameObject() = default;

	virtual void Render(const SpriteRenderer& renderer);
	
private:
	glm::vec2 m_position;
	glm::vec2 m_size;
	glm::vec3 m_color;
	std::shared_ptr<Texture> m_sprite;
};