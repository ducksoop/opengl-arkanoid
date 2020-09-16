#pragma once

#include "SpriteRenderer.h"
#include "Brick.h"

#include <string>
#include <vector>

class Level
{
public:
	Level(const std::string& path, int levelWidth, int levelHeight);
	~Level() = default;

	void Render(SpriteRenderer& renderer);
	bool IsCleared();

	std::vector<Brick>& GetBricks();
	
private:
	std::vector<std::vector<int>> LoadTiles(const std::string& path);
	void InitializeLevel(std::vector<std::vector<int>> tiles, int levelWidth, int levelHeight);
	
	std::vector<Brick> m_bricks;
};