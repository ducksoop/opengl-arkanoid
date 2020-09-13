#include "Level.h"
#include "FileManager.h"
#include "ResourceManager.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <string>
#include <sstream>
#include <iostream>

Level::Level(const std::string& path, int levelWidth, int levelHeight)
{
	auto tiles = LoadTiles(path);
	InitializeLevel(tiles, levelWidth, levelHeight);
}

void Level::Render(SpriteRenderer& renderer)
{
	for (auto& brick : m_bricks)
		brick.Render(renderer);
}

bool Level::IsCleared()
{
	return false;
}

std::vector<std::vector<int>> Level::LoadTiles(const std::string& path)
{
	std::vector<std::vector<int>> tiles;

	int tile;
	std::stringstream level(FileManager::Instance().ReadAsText(path));
	std::string line;

	while (std::getline(level, line))
	{
		std::stringstream rowStream(line);
		std::vector<int> row;
		while (rowStream >> tile)
		{
			row.push_back(tile);
		}
		tiles.push_back(row);
	}

	return tiles;
}

void Level::InitializeLevel(std::vector<std::vector<int>> tiles, int levelWidth, int levelHeight)
{
	float tileWidth = static_cast<float>(levelWidth) / static_cast<float>(tiles[0].size());
	float tileHeight = static_cast<float>(levelHeight) / static_cast<float>(tiles.size());

	for (unsigned int i = 0; i < tiles.size(); ++i)
	{
		for (unsigned int j = 0; j < tiles[0].size(); ++j)
		{
			glm::vec2 position(tileWidth * j, tileHeight * i);
			glm::vec2 size(tileWidth, tileHeight);

			switch (tiles[i][j])
			{
			case 1:
				m_bricks.emplace_back(position, size, glm::vec3(0.8f, 0.8f, 0.7f),
				                      ResourceManager::Instance().GetTexture("block_solid"),
				                      true);
				break;
			case 2:
				m_bricks.emplace_back(position, size, glm::vec3(0.2f, 0.6f, 1.0f),
				                      ResourceManager::Instance().GetTexture("block"),
				                      false);
				break;
			case 3:
				m_bricks.emplace_back(position, size, glm::vec3(0.0f, 0.7f, 0.0f),
				                      ResourceManager::Instance().GetTexture("block"),
				                      false);
				break;
			case 4:
				m_bricks.emplace_back(position, size, glm::vec3(0.8f, 0.8f, 0.4f),
				                      ResourceManager::Instance().GetTexture("block"),
				                      false);
				break;
			case 5:
				m_bricks.emplace_back(position, size, glm::vec3(1.0f, 0.5f, 0.0f),
				                      ResourceManager::Instance().GetTexture("block"),
				                      false);
				break;
			default:
				break;
			}
		}
	}
}
