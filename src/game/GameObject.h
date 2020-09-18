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
        Texture* sprite);
	virtual ~GameObject() = default;

	virtual void Render(const SpriteRenderer& renderer);

	const glm::vec2& GetSize() const;
	const glm::vec2& GetPosition() const;
	const glm::vec3& GetColor() const;

	void SetPosition(const glm::vec2& position);
	void SetPositionX(float x);
	void SetPositionY(float y);
	void UpdatePositionX(float delta);
	void UpdatePositionY(float delta);

	void SetSize(const glm::vec2& size);
	void SetSizeX(float x);
	void SetSizeY(float y);
	void UpdateSizeX(float delta);
	void UpdateSizeY(float delta);

	void SetColor(const glm::vec3& color);

protected:
	glm::vec2 m_position;
	glm::vec2 m_size;
	glm::vec3 m_color;
	Texture* m_sprite;
};