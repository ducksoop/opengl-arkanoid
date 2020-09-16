#include "CollisionDetector.h"

Collision CollisionDetector::CheckCollisionAABB(const Ball& ball, const GameObject& gameObject)
{
	// Get center point on circle first
	glm::vec2 center = ball.GetPosition() + ball.GetRadius();

	// Calculate AABB info
	glm::vec2 aabbHalfExtents(gameObject.GetSize().x / 2, gameObject.GetSize().y / 2);
	glm::vec2 aabbCenter(
		gameObject.GetPosition().x + aabbHalfExtents.x,
		gameObject.GetPosition().y + aabbHalfExtents.y
	);

	// Get difference vector between both centers
	glm::vec2 difference = center - aabbCenter;
	glm::vec2 clamped = glm::clamp(difference, -aabbHalfExtents, aabbHalfExtents);

	// Add clamped value to AABB center and we get the value of box closes to circle
	glm::vec2 closest = aabbCenter + clamped;

	// Retrieve vector between circle and closest point AABB and check if length is less than or equal to radius
	difference = closest - center;

	return glm::length(difference) <= ball.GetRadius() ?
		std::make_tuple(true, GetVectorDirection(difference), difference) :
		std::make_tuple(false, Direction::Up, glm::vec2(0, 0));
}

Direction CollisionDetector::GetVectorDirection(const glm::vec2& target)
{
	glm::vec2 compass[] =
	{
		glm::vec2(0.0f, 1.0f),		// up
		glm::vec2(0.0f, -1.0f),	// down
		glm::vec2(-1.0f, 0.0f),	// left
		glm::vec2(1.0f, 0.0f)		// right
	};

	float max = 0.0f;
	int bestMatch = -1;
	for (GLuint i = 0; i < 4; i++)
	{
		float dotProduct = glm::dot(glm::normalize(target), compass[i]);
		if (dotProduct > max)
		{
			max = dotProduct;
			bestMatch = i;
		}
	}

	return static_cast<Direction>(bestMatch);
}
