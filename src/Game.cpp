#include "Game.h"
#include "GameState.h"
#include "ResourceManager.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "ShaderType.h"
#include "Window.h"

#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <utility>

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

	if (m_inputManager.IsKeyPressed(GLFW_KEY_A) || m_inputManager.IsKeyPressed(GLFW_KEY_LEFT))
	{
		if (m_player->m_position.x >= m_player->m_boundaries.x)
		{
			m_player->m_position.x -= m_player->m_velocity * dt;
			if (m_ball->m_isStuck)
			{
				m_ball->m_position.x -= m_player->m_velocity * dt;
			}
		}
	}
	if (m_inputManager.IsKeyPressed(GLFW_KEY_D) || m_inputManager.IsKeyPressed(GLFW_KEY_RIGHT))
	{
		if (m_player->m_position.x <= m_player->m_boundaries.y)
		{
			m_player->m_position.x += m_player->m_velocity * dt;
			if (m_ball->m_isStuck)
			{
				m_ball->m_position.x += m_player->m_velocity * dt;
			}
		}
	}

	if (m_inputManager.IsKeyPressed(GLFW_KEY_SPACE))
	{
		m_ball->m_isStuck = false;
	}
}

void Game::Update(GLfloat dt)
{
	m_ball->Move(glm::vec4(0.0f, m_window->GetWidth(), 0.0f, m_window->GetHeight()), dt);
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
	auto shader = m_resourceManager.CreateShaderProgram("sprite",
		Shader(ShaderType::Vertex,
			"../res/shaders/sprite/shader.vert"),
		Shader(ShaderType::Fragment,
			"../res/shaders/sprite/shader.frag"));
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(m_window->GetWidth()),
		static_cast<GLfloat>(m_window->GetHeight()), 0.0f,
		-1.0f, 1.0f);

	shader->Use();
	shader->SetUniform("projection", projection);
	shader->SetUniform("image", 0);

	m_spriteRenderer.Initialize(shader);

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

	m_levels.push_back(std::make_shared<Level>(
		"../res/levels/1.txt", m_window->GetWidth(), m_window->GetHeight() / 2));
	m_levels.push_back(std::make_shared<Level>(
		"../res/levels/2.txt", m_window->GetWidth(), m_window->GetHeight() / 2));
	m_levels.push_back(std::make_shared<Level>(
		"../res/levels/3.txt", m_window->GetWidth(), m_window->GetHeight() / 2));
	m_levels.push_back(std::make_shared<Level>(
		"../res/levels/4.txt", m_window->GetWidth(), m_window->GetHeight() / 2));
	m_currentLevel = 0;

	glm::vec2 playerSize = glm::vec2(120, 20);

	glm::vec2 playerPosition = glm::vec2(
		m_window->GetWidth() / 2 - playerSize.x / 2,
		m_window->GetHeight() - playerSize.y
	);

	m_player = std::make_shared<Paddle>(playerPosition,
	                                    playerSize,
	                                    glm::vec3(1.0f),
	                                    m_resourceManager.GetTexture("paddle"),
	                                    500.0f,
	                                    glm::vec2(0, m_window->GetWidth() - playerSize.x)
	);

	float ballRadius = 15.0f;
	glm::vec2 ballVelocity = glm::vec2(200.0f, -500.0f);

	m_ball = std::make_shared<Ball>(playerPosition + glm::vec2(playerSize.x / 2 - ballRadius,
	                                                           -2 * ballRadius),
	                                ballRadius,
	                                glm::vec3(1.0f),
	                                m_resourceManager.GetTexture("face"),
	                                ballVelocity
	);
}
