#include "Game.h"
#include "GameState.h"
#include "ResourceManager.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "ShaderType.h"
#include "Window.h"
#include "CollisionDetector.h"

#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <utility>
#include <tuple>

namespace {
	const glm::vec2 INITIAL_BALL_VELOCITY(250.0f, -350.0f);
	const float INITIAL_PLAYER_VELOCITY = 500.0f;
}

Game::Game(int w, int h, bool isFullscreen)
	: m_gameState(GameState::GameActive)
{
	m_windowManager.Initialize();
	m_inputManager.Initialize();
	m_resourceManager.Initialize();
	
	InitializeWindow(w, h, isFullscreen);
	InitializeOpenGL();
	InitializeResources();
}

Game::~Game()
{
	m_resourceManager.Close();
	m_inputManager.Close();
	m_windowManager.Close();
}

void Game::HandleInput(GLfloat dt)
{
	m_inputManager.ProcessEvents(dt);
	float velocity = m_player->GetVelocity() * dt;

	if (m_inputManager.IsKeyPressed(GLFW_KEY_A) || m_inputManager.IsKeyPressed(GLFW_KEY_LEFT))
	{
		if (m_player->GetPosition().x >= m_player->GetBoundaries().x)
		{
			m_player->UpdatePositionX(-velocity);
			if (m_ball->IsStuck())
			{
				m_ball->UpdatePositionX(-velocity);
			}
		}
	}
	if (m_inputManager.IsKeyPressed(GLFW_KEY_D) || m_inputManager.IsKeyPressed(GLFW_KEY_RIGHT))
	{
		if (m_player->GetPosition().x <= m_player->GetBoundaries().y)
		{
			m_player->UpdatePositionX(velocity);
			if (m_ball->IsStuck())
			{
				m_ball->UpdatePositionX(velocity);
			}
		}
	}

	if (m_inputManager.IsKeyPressed(GLFW_KEY_SPACE))
	{
		m_ball->SetIsStuck(false);
	}
}

void Game::Update(GLfloat dt)
{
	m_ball->Update(dt);
	m_particleEmitter->Update(dt, *m_ball, 5, glm::vec2(m_ball->GetRadius() / 2));
	CheckCollisions();
}

void Game::Render()
{
	if (m_gameState == GameState::GameActive) 
	{
		m_spriteRenderer.RenderSprite(m_resourceManager.GetTexture("background"),
		                              glm::vec2(0.0f, 0.0f),
		                              glm::vec2(m_window->GetWidth(), m_window->GetHeight()));

		m_levels[m_currentLevel]->Render(m_spriteRenderer);
		m_player->Render(m_spriteRenderer);
		m_particleEmitter->Render();
		m_ball->Render(m_spriteRenderer);
	}

	m_window->SwapBuffers();
}

bool Game::IsExiting()
{
	return m_window->IsClosing();
}

void Game::InitializeWindow(int w, int h, bool isFullscreen)
{
	m_window = m_windowManager.CreateWindow(w, h, "Arkanoid", isFullscreen);

	m_inputManager.AddKeyHandler("exit", [this](float delta) 
	{
		if (m_inputManager.IsKeyPressed(GLFW_KEY_ESCAPE)) 
		{
			m_window->SetShouldClose(true);
		}
	});
}

void Game::InitializeOpenGL()
{
	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Game::InitializeResources()
{
	auto spriteShader = m_resourceManager.CreateShaderProgram("sprite",
		Shader(ShaderType::Vertex,
			"../res/shaders/sprite/shader.vert"),
		Shader(ShaderType::Fragment,
			"../res/shaders/sprite/shader.frag"));
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(m_window->GetWidth()),
		static_cast<GLfloat>(m_window->GetHeight()), 0.0f,
		-1.0f, 1.0f);

	spriteShader->Use();
	spriteShader->SetUniform("projection", projection);
	spriteShader->SetUniform("sprite", 0);

	auto particleShader = m_resourceManager.CreateShaderProgram("particle",
		Shader(ShaderType::Vertex,
			"../res/shaders/particle/shader.vert"),
		Shader(ShaderType::Fragment,
			"../res/shaders/particle/shader.frag"));

	particleShader->Use();
	particleShader->SetUniform("projection", projection);
	particleShader->SetUniform("sprite", 0);

	m_spriteRenderer.Initialize(spriteShader);

	m_resourceManager.CreateTexture("background",
	                                "../res/textures/background.jpg",
	                                1600, 900);
	m_resourceManager.CreateTexture("face",
	                                "../res/textures/awesome_face.png",
	                                512, 512, 4, GL_RGBA);
	m_resourceManager.CreateTexture("block",
	                                "../res/textures/block.png",
	                                128, 128);
	m_resourceManager.CreateTexture("block_solid",
	                                "../res/textures/block_solid.png",
	                                128, 128);
	m_resourceManager.CreateTexture("paddle",
	                                "../res/textures/paddle.png",
	                                512, 128, 4, GL_RGBA);
	m_resourceManager.CreateTexture("particle",
	                                "../res/textures/particle.png",
	                                500, 500, 4, GL_RGBA);

	m_particleEmitter = std::make_unique<ParticleEmitter>(m_resourceManager.GetShaderProgram("particle"),
	                                                      m_resourceManager.GetTexture("particle"),
	                                                      500);

	m_levels.push_back(std::make_unique<Level>(
		"../res/levels/1.txt", m_window->GetWidth(), m_window->GetHeight() / 2));
	m_levels.push_back(std::make_unique<Level>(
		"../res/levels/2.txt", m_window->GetWidth(), m_window->GetHeight() / 2));
	m_levels.push_back(std::make_unique<Level>(
		"../res/levels/3.txt", m_window->GetWidth(), m_window->GetHeight() / 2));
	m_levels.push_back(std::make_unique<Level>(
		"../res/levels/4.txt", m_window->GetWidth(), m_window->GetHeight() / 2));
	m_currentLevel = 0;

	glm::vec2 playerSize = glm::vec2(120, 20);

	glm::vec2 playerPosition = glm::vec2(
		m_window->GetWidth() / 2 - playerSize.x / 2,
		m_window->GetHeight() - playerSize.y
	);

	m_player = std::make_unique<Paddle>(playerPosition,
	                                    playerSize,
	                                    glm::vec3(1.0f),
	                                    m_resourceManager.GetTexture("paddle"),
	                                    INITIAL_PLAYER_VELOCITY,
	                                    glm::vec2(0, m_window->GetWidth() - playerSize.x)
	);

	float ballRadius = 15.0f;
	m_ball = std::make_unique<Ball>(playerPosition + glm::vec2(playerSize.x / 2 - ballRadius,
	                                                           -2 * ballRadius),
	                                ballRadius,
	                                glm::vec3(1.0f),
	                                m_resourceManager.GetTexture("face"),
	                                INITIAL_BALL_VELOCITY,
									glm::vec4(0.0f, m_window->GetWidth(), 0.0f, m_window->GetHeight())
	);
}

void Game::CheckCollisions()
{
	// Check collisions with the bricks
	for (auto& brick : m_levels[m_currentLevel]->GetBricks())
	{
		if (brick->IsDestroyed())
			continue;

		// There was no collision
		Collision collision = CollisionDetector::CheckCollisionAABB(*m_ball, *brick);
		if (!std::get<0>(collision))
			continue;

		if (!brick->IsSolid())
			brick->SetIsDestroyed(true);

		// Collision resolution
		Direction direction = std::get<1>(collision);
		glm::vec2 difference = std::get<2>(collision);

		if (direction == Direction::Left || direction == Direction::Right)
		{
			m_ball->SetVelocityX(-m_ball->GetVelocity().x);

			// Relocate
			float penetration = m_ball->GetRadius() - std::abs(difference.x);
			if (direction == Direction::Left)
			{
				m_ball->UpdatePositionX(penetration); // Move ball to right
			}
			else
			{
				m_ball->UpdatePositionX(-penetration); // Move ball to left
			}
		}
		else
		{
			m_ball->SetVelocityY(-m_ball->GetVelocity().y);
			float penetration = m_ball->GetRadius() - std::abs(difference.y);
			if (direction == Direction::Up)
			{
				m_ball->UpdatePositionY(-penetration);	// Move ball up
			}
			else
			{
				m_ball->UpdatePositionY(penetration);	// Move ball down
			}
		}
	}

	// Check collision with the paddle
	Collision collision = CollisionDetector::CheckCollisionAABB(*m_ball, *m_player);
	if (std::get<0>(collision) && !m_ball->IsStuck())
	{
		// Check where it hit the board, and change the velocity based on where it hit
		float centerBoard = m_player->GetPosition().x + m_player->GetSize().x / 2;
		float distance = (m_ball->GetPosition().x + m_ball->GetRadius()) - centerBoard;
		float percentage = distance / (m_player->GetSize().x / 2);

		// Then move accordingly
		float strength = 2.0f;
		glm::vec2 oldVelocity = m_ball->GetVelocity();
		m_ball->SetVelocityX(INITIAL_BALL_VELOCITY.x * percentage * strength);
		m_ball->SetVelocityY(-1 * std::abs(m_ball->GetVelocity().y));
		m_ball->SetVelocity(glm::normalize(m_ball->GetVelocity()) * glm::length(oldVelocity));
	}
}
