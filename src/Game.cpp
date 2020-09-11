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

Game::Game(int w, int h)
	: m_gameState(GameState::GameActive)
	, m_window()
{
	m_windowManager.Initialize();
	m_inputManager.Initialize();
	m_resourceManager.Initialize();
	
	InitializeWindow(w, h);
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
	this->m_inputManager.ProcessEvents();
}

void Game::Update(GLfloat dt)
{
	
}

void Game::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	this->m_spriteRenderer.RenderSprite(this->m_resourceManager.GetTexture("awesomeFace"),
	                              glm::vec2(200, 200),
	                              glm::vec2(300, 400),
	                              glm::vec3(0.0f, 1.0f, 0.0f),
	                              glm::radians(45.0f));

	m_window->SwapBuffers();
}

bool Game::IsExiting()
{
	return m_window->IsClosing();
}

void Game::InitializeWindow(int w, int h)
{
	m_window = this->m_windowManager.CreateWindow(w, h, "Breakout");

	this->m_inputManager.AddKeyHandler("exit", [this](int key, int scanCode, int action, int mods) 
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
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
	auto shader = this->m_resourceManager.CreateShaderProgram("sprite",
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

	this->m_spriteRenderer.Initialize(shader);
	this->m_resourceManager.CreateTexture("awesomeFace",
		"../res/textures/awesome_face.png",
		512, 512, 4, GL_RGBA);
}
