#pragma once

#include "GameState.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Window.h"

#include <GL/glew.h>

#include <memory>

class Game
{
public:
	Game(std::shared_ptr<Window> window);
	~Game() = default;

	void HandleInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();
	
private:
	void Initialize(int w, int h);
	void InitializeOpenGL();
	
	GameState m_gameState;
	std::shared_ptr<Window> m_window;

	InputManager& inputManager = InputManager::Instance();
	ResourceManager& resourceManager = ResourceManager::Instance();
	SpriteRenderer& spriteRenderer = SpriteRenderer::Instance();
};