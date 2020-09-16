#pragma once

#include "GameObject.h"

class Brick : public GameObject
{
public:
    Brick(const glm::vec2& position,
        const glm::vec2& size,
        const glm::vec3& color,
        Texture* sprite,
		bool isSolid = false);
	~Brick() = default;

    bool IsSolid() const;
    bool IsDestroyed() const;
    void SetIsDestroyed(bool flag);
	
private:
    bool m_isSolid;
    bool m_isDestroyed;
};