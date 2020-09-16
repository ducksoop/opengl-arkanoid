#include "Brick.h"

#include <utility>

Brick::Brick(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color, Texture* sprite, bool isSolid)
	: GameObject(position, size, color, sprite)
	, m_isSolid(isSolid)
	, m_isDestroyed(false)
{
}

bool Brick::IsSolid() const
{
	return m_isSolid;
}

bool Brick::IsDestroyed() const
{
	return m_isDestroyed;
}

void Brick::SetIsDestroyed(bool flag)
{
	m_isDestroyed = flag;
}
