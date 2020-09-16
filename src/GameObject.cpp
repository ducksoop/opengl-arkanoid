#include "GameObject.h"

#include <iostream>

GameObject::GameObject(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color, std::shared_ptr<Texture> sprite)
	: m_position(position)
	, m_size(size)
	, m_color(color)
	, m_sprite(sprite)
{
}

void GameObject::Render(const SpriteRenderer& renderer)
{
	renderer.RenderSprite(m_sprite, m_position, m_size, m_color);
}

const glm::vec2& GameObject::GetSize() const
{
	return m_size;
}

const glm::vec2& GameObject::GetPosition() const
{
	return m_position;
}

const glm::vec2& GameObject::GetColor() const
{
	return m_color;
}

void GameObject::SetPosition(const glm::vec2& position)
{
	m_position = position;
}

void GameObject::SetPositionX(float x)
{
	m_position.x = x;
}

void GameObject::SetPositionY(float y)
{
	m_position.y = y;
}

void GameObject::UpdatePositionX(float delta)
{
	m_position.x += delta;
}

void GameObject::UpdatePositionY(float delta)
{
	m_position.y += delta;
}
