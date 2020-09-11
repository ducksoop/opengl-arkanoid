#pragma once

#include <GL/glew.h>

#include "GameState.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "WindowManager.h"
#include "Window.h"

#include <memory>

class Game
{
public:
	Game(int w, int h);
	~Game();

	void HandleInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();

	bool IsExiting();
	
private:
	void InitializeWindow(int w, int h);
	void InitializeOpenGL();
	void InitializeResources();
	
	GameState m_gameState;
	std::shared_ptr<Window> m_window;

	WindowManager& m_windowManager = WindowManager::Instance();
	InputManager& m_inputManager = InputManager::Instance();
	ResourceManager& m_resourceManager = ResourceManager::Instance();
	SpriteRenderer& m_spriteRenderer = SpriteRenderer::Instance();
};