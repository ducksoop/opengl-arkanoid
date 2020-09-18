#include "GameObject.h"

#include <iostream>

GameObject::GameObject(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color, Texture* sprite)
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

const glm::vec3& GameObject::GetColor() const
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

void GameObject::SetSize(const glm::vec2& size)
{
	m_size = size;
}

void GameObject::SetSizeX(float x)
{
	m_size.x = x;
}

void GameObject::SetSizeY(float y)
{
	m_size.y = y;
}

void GameObject::UpdateSizeX(float delta)
{
	m_size.x += delta;
}

void GameObject::UpdateSizeY(float delta)
{
	m_size.y += delta;
}

void GameObject::SetColor(const glm::vec3& color)
{
	m_color = color;
}
