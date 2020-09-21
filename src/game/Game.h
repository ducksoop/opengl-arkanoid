#pragma once

#include <GL/glew.h>

#include "GameState.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "WindowManager.h"
#include "AudioManager.h"
#include "Window.h"
#include "Level.h"
#include "Paddle.h"
#include "Ball.h"
#include "Powerup.h"
#include "Collision.h"
#include "ParticleEmitter.h"
#include "PostProcessing.h"

#include <memory>
#include <vector>

class Game
{
public:
	Game(int w, int h, bool isFullscreen);
	~Game();

	void HandleInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();

	bool IsExiting();
	
private:
	void InitializeWindow(int w, int h, bool isFullscreen);
	void InitializeOpenGL();
	void InitializeResources();

	void CheckCollisions();
	void SpawnPowerups(const Brick& brick);
	
	GameState m_gameState;
	Window* m_window;
	std::vector<std::unique_ptr<Level>> m_levels;
	std::vector<std::unique_ptr<Powerup>> m_powerups;
	int m_currentLevel;

	std::unique_ptr<Paddle> m_player;
	std::unique_ptr<Ball> m_ball;
	std::unique_ptr<ParticleEmitter> m_particleEmitter;
	std::unique_ptr<PostProcessing> m_postProcessing;

	WindowManager& m_windowManager = WindowManager::Instance();
	InputManager& m_inputManager = InputManager::Instance();
	ResourceManager& m_resourceManager = ResourceManager::Instance();
	SpriteRenderer& m_spriteRenderer = SpriteRenderer::Instance();
	AudioManager& m_audioManager = AudioManager::Instance();
};
