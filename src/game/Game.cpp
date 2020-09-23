#include "Game.h"
#include "GameState.h"
#include "ResourceManager.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "ShaderType.h"
#include "Window.h"
#include "CollisionDetector.h"
#include "Random.h"

#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <utility>
#include <tuple>
#include <algorithm>

namespace {
	const int GAME_WIDTH = 960;
	const int GAME_HEIGHT = 540;
	const glm::vec2 INITIAL_BALL_VELOCITY(250.0f, -350.0f);
	const float INITIAL_PLAYER_VELOCITY = 600.0f;
	GLfloat shakeTime = 0.0f;
}

Game::Game(int w, int h, bool isFullscreen)
	: m_gameState(GameState::GameActive)
	, m_lives(3)
	, m_scales(static_cast<float>(w) / GAME_WIDTH, static_cast<float>(h) / GAME_HEIGHT)
{
	m_windowManager.Initialize();
	m_inputManager.Initialize();
	m_resourceManager.Initialize();
	m_audioManager.Initialize();
	
	InitializeWindow(w, h, isFullscreen);
	InitializeOpenGL();
	InitializeResources();

	m_audioManager.PlayAudioSource("background");
}

Game::~Game()
{
	m_audioManager.Close();
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
	m_player->SetBoundaries(glm::vec2(0, m_window->GetWidth() - m_player->GetSize().x));
	m_ball->Update(dt);
	m_particleEmitter->Update(dt, *m_ball, 5, glm::vec2(m_ball->GetRadius() / 2));
	CheckCollisions();

	if (m_ball->GetPosition().y >= m_window->GetHeight() - m_ball->GetSize().y)
	{
		m_lives--;

		// Game over when lives are 0
	}

	if (shakeTime > 0.0f)
	{
		shakeTime -= dt;
		if (shakeTime <= 0.0f)
			m_postProcessing->DisableEffects(PostProcessingEffect::Shake);
	}

	for (auto& pickUp : m_powerups)
		pickUp->Update(dt, *m_player, *m_ball, *m_postProcessing);

	m_powerups.erase(std::remove_if(m_powerups.begin(), m_powerups.end(), [](std::unique_ptr<Powerup> const& pickUp)
	{
		return pickUp->IsDestroyed() && !pickUp->IsActivated();
	}), m_powerups.end());
}

void Game::Render()
{
	if (m_gameState == GameState::GameActive) 
	{
		m_postProcessing->BeginRender();
		
		m_spriteRenderer.RenderSprite(m_resourceManager.GetTexture("background"),
		                              glm::vec2(0.0f, 0.0f),
		                              glm::vec2(m_window->GetWidth(), m_window->GetHeight()));

		m_levels[m_currentLevel]->Render(m_spriteRenderer);
		m_player->Render(m_spriteRenderer);
		m_particleEmitter->Render(m_ball->GetRadius());
		m_ball->Render(m_spriteRenderer);

		for (auto& pickUp : m_powerups)
		{
			if (!pickUp->IsDestroyed())
				pickUp->Render(m_spriteRenderer);
		}

		m_textRenderer.Render("Lives: " + std::to_string(m_lives), glm::vec2(10.0f, 30.0f) * m_scales, glm::vec3(1.0f, 1.0f, 1.0f), 0.6f * glm::length(m_scales));

		m_postProcessing->EndRender();
		m_postProcessing->Render(static_cast<GLfloat>(glfwGetTime()), glm::length(m_scales));
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
	auto* spriteShader = m_resourceManager.CreateShaderProgram("sprite",
	                                                           Shader(Vertex,
	                                                                  "../res/shaders/sprite/shader.vert"),
	                                                           Shader(Fragment,
	                                                                  "../res/shaders/sprite/shader.frag"));
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(m_window->GetWidth()),
	                                  static_cast<GLfloat>(m_window->GetHeight()), 0.0f,
	                                  -1.0f, 1.0f);

	spriteShader->Use();
	spriteShader->SetUniform("projection", projection);
	spriteShader->SetUniform("sprite", 0);

	m_spriteRenderer.Initialize(spriteShader);

	auto* textRenderingShader = m_resourceManager.CreateShaderProgram("text",
	                                                                  Shader(Vertex,
	                                                                         "../res/shaders/text/shader.vert"),
	                                                                  Shader(Fragment,
	                                                                         "../res/shaders/text/shader.frag"));

	textRenderingShader->Use();
	textRenderingShader->SetUniform("projection", projection);
	m_textRenderer.Initialize("../res/fonts/manaspc.ttf", textRenderingShader);

	auto* particleShader = m_resourceManager.CreateShaderProgram("particle",
	                                                             Shader(Vertex,
	                                                                    "../res/shaders/particle/shader.vert"),
	                                                             Shader(Fragment,
	                                                                    "../res/shaders/particle/shader.frag"));

	particleShader->Use();
	particleShader->SetUniform("projection", projection);
	particleShader->SetUniform("sprite", 0);

	m_resourceManager.CreateShaderProgram("postprocessing",
	                                      Shader(Vertex,
	                                             "../res/shaders/postprocessing/shader.vert"),
	                                      Shader(Fragment,
	                                             "../res/shaders/postprocessing/shader.frag"));
	
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
	m_resourceManager.CreateTexture("speedUp",
	                                "../res/textures/powerups/powerup_speedup.png",
	                                512, 128, 4, GL_RGBA);
	m_resourceManager.CreateTexture("sticky",
	                                "../res/textures/powerups/powerup_sticky.png",
	                                512, 128, 4, GL_RGBA);
	m_resourceManager.CreateTexture("passThrough",
	                                "../res/textures/powerups/powerup_passthrough.png",
	                                512, 128, 4, GL_RGBA);
	m_resourceManager.CreateTexture("padSizeIncrease",
	                                "../res/textures/powerups/powerup_increase.png",
	                                512, 128, 4, GL_RGBA);
	m_resourceManager.CreateTexture("confuse",
	                                "../res/textures/powerups/powerup_confuse.png",
	                                512, 128, 4, GL_RGBA);
	m_resourceManager.CreateTexture("chaos",
	                                "../res/textures/powerups/powerup_chaos.png",
	                                512, 128, 4, GL_RGBA);

	m_audioManager.CreateAudioSource("background", "../res/audio/background.ogg", true);
	m_audioManager.CreateAudioSource("bleep", "../res/audio/bleep.ogg", false);
	m_audioManager.CreateAudioSource("bleepPaddle", "../res/audio/bleep_paddle.ogg", false);
	m_audioManager.CreateAudioSource("solid", "../res/audio/solid.ogg", false);
	m_audioManager.CreateAudioSource("powerup", "../res/audio/powerup.ogg", false);

	m_particleEmitter = std::make_unique<ParticleEmitter>(m_resourceManager.GetShaderProgram("particle"),
	                                                      m_resourceManager.GetTexture("particle"),
	                                                      500);
	m_postProcessing = std::make_unique<PostProcessing>(m_resourceManager.GetShaderProgram("postprocessing"),
	                                                    m_window->GetWidth(), m_window->GetHeight());

	m_levels.push_back(std::make_unique<Level>(
		"../res/levels/1.txt", m_window->GetWidth(), m_window->GetHeight() / 2));
	m_levels.push_back(std::make_unique<Level>(
		"../res/levels/2.txt", m_window->GetWidth(), m_window->GetHeight() / 2));
	m_levels.push_back(std::make_unique<Level>(
		"../res/levels/3.txt", m_window->GetWidth(), m_window->GetHeight() / 2));
	m_levels.push_back(std::make_unique<Level>(
		"../res/levels/4.txt", m_window->GetWidth(), m_window->GetHeight() / 2));
	m_currentLevel = 3;

	glm::vec2 playerSize = glm::vec2(150, 20) * m_scales;

	glm::vec2 playerPosition = glm::vec2(
		m_window->GetWidth() / 2 - playerSize.x / 2,
		m_window->GetHeight() - playerSize.y
	);

	m_player = std::make_unique<Paddle>(playerPosition,
	                                    playerSize,
	                                    glm::vec3(1.0f),
	                                    m_resourceManager.GetTexture("paddle"),
	                                    INITIAL_PLAYER_VELOCITY * m_scales.x,
	                                    glm::vec2(0, m_window->GetWidth() - playerSize.x)
	);

	float ballRadius = 10.0f * glm::length(m_scales);
	m_ball = std::make_unique<Ball>(playerPosition + glm::vec2(playerSize.x / 2 - ballRadius,
	                                                           -2 * ballRadius),
	                                ballRadius,
	                                glm::vec3(1.0f),
	                                m_resourceManager.GetTexture("face"),
	                                INITIAL_BALL_VELOCITY * m_scales,
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
		Collision collision = CollisionDetector::CheckCollisionAABB_Circle(*m_ball, *brick);
		if (!std::get<0>(collision))
			continue;

		if (brick->IsSolid())
		{
			m_audioManager.PlayAudioSource("solid");
			shakeTime = 0.2f;
			m_postProcessing->EnableEffects(PostProcessingEffect::Shake);
		}
		else
		{
			m_audioManager.PlayAudioSource("bleep");
			brick->SetIsDestroyed(true);
			SpawnPowerups(*brick);
		}

		if (m_ball->IsPassingThrough())
			continue;

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
	Collision collision = CollisionDetector::CheckCollisionAABB_Circle(*m_ball, *m_player);
	if (std::get<0>(collision) && !m_ball->IsStuck())
	{
		m_audioManager.PlayAudioSource("bleepPaddle");
		
		// Check where it hit the board, and change the velocity based on where it hit
		float centerBoard = m_player->GetPosition().x + m_player->GetSize().x / 2;
		float distance = (m_ball->GetPosition().x + m_ball->GetRadius()) - centerBoard;
		float percentage = distance / (m_player->GetSize().x / 2);

		// Then move accordingly
		float strength = 2.0f;
		glm::vec2 oldVelocity = m_ball->GetVelocity();
		m_ball->SetVelocityX(INITIAL_BALL_VELOCITY.x * m_scales.x * percentage * strength);
		m_ball->SetVelocityY(-1 * std::abs(m_ball->GetVelocity().y));
		m_ball->SetVelocity(glm::normalize(m_ball->GetVelocity()) * glm::length(oldVelocity));

		m_ball->SetIsStuck(m_ball->IsSticky());
	}

	// Check collision between powerups and the paddle
	for (auto& powerup : m_powerups)
	{
		if (powerup->IsDestroyed())
			continue;

		if (powerup->GetPosition().y >= m_window->GetHeight())
			powerup->SetIsDestroyed(true);

		if (CollisionDetector::CheckCollisionAABB_AABB(*m_player, *powerup))
		{
			m_audioManager.PlayAudioSource("powerup");
			powerup->Activate(*m_player, *m_ball, *m_postProcessing, m_scales);
		}
	}
}

void Game::SpawnPowerups(const Brick& brick)
{
	const glm::vec2 size = glm::vec2(60, 20) * m_scales;
	const glm::vec2 velocity = glm::vec2(0.0f, 150.0f) * m_scales;
	
	// Positive powerups (these spawn less frequently)
	if (Random::Chance(50))
	{
		m_powerups.push_back(std::make_unique<Powerup>(brick.GetPosition(), size, glm::vec3(0.5f, 0.5f, 1.0f),
		                                               m_resourceManager.GetTexture("speedUp"), velocity, PowerupType::SpeedUp,
		                                               0.0f));
	}

	if (Random::Chance(50))
	{
		m_powerups.push_back(std::make_unique<Powerup>(brick.GetPosition(), size, glm::vec3(1.0f, 0.5f, 1.0f),
			m_resourceManager.GetTexture("sticky"), velocity, PowerupType::Sticky,
			20.0f));
	}

	if (Random::Chance(50))
	{
		m_powerups.push_back(std::make_unique<Powerup>(brick.GetPosition(), size, glm::vec3(1.0f, 0.5f, 1.0f),
			m_resourceManager.GetTexture("passThrough"), velocity, PowerupType::PassThrough,
			10.0f));
	}

	if (Random::Chance(50))
	{
		m_powerups.push_back(std::make_unique<Powerup>(brick.GetPosition(), size, glm::vec3(1.0f, 0.6f, 0.4f),
			m_resourceManager.GetTexture("padSizeIncrease"), velocity, PowerupType::PaddleSizeIncrease,
			0.0f));
	}

	// Negative powerups (spawn more frequently)
	if (Random::Chance(15))
	{
		m_powerups.push_back(std::make_unique<Powerup>(brick.GetPosition(), size, glm::vec3(1.0, 0.3f, 0.3f),
			m_resourceManager.GetTexture("confuse"), velocity, PowerupType::Confuse,
			15.0f));
	}

	if (Random::Chance(15))
	{
		m_powerups.push_back(std::make_unique<Powerup>(brick.GetPosition(), size, glm::vec3(0.9f, 0.25f, 0.25f),
			m_resourceManager.GetTexture("chaos"), velocity, PowerupType::Chaos,
			15.0f));
	}
}
