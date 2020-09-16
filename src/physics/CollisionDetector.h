#pragma once

#include "Collision.h"
#include "Direction.h"
#include "GameObject.h"
#include "Ball.h"

class CollisionDetector
{
public:
	static Collision CheckCollisionAABB(const Ball& ball, const GameObject& gameObject);
private:
	static Direction GetVectorDirection(const glm::vec2& target);
};
