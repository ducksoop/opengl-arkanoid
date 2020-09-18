#pragma once

#include "Collision.h"
#include "Direction.h"
#include "GameObject.h"
#include "Ball.h"

class CollisionDetector
{
public:
	static Collision CheckCollisionAABB_Circle(const Ball& ball, const GameObject& gameObject);
	static bool CheckCollisionAABB_AABB(const GameObject& gameObject1, const GameObject& gameObject2);
private:
	static Direction GetVectorDirection(const glm::vec2& target);
};
