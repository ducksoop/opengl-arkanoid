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

Game::Game(std::shared_ptr<Window> window)
	: m_gameState(GameState::GameActive)
	, m_window(window)
{
	InitializeOpenGL();
	Initialize(m_window->GetWidth(), m_window->GetHeight());
}

void Game::HandleInput(GLfloat dt)
{
	this->inputManager.ProcessEvents();
}

void Game::Update(GLfloat dt)
{
	
}

void Game::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	this->spriteRenderer.RenderSprite(this->resourceManager.GetTexture("awesomeFace"),
	                              glm::vec2(200, 200),
	                              glm::vec2(300, 400),
	                              glm::vec3(0.0f, 1.0f, 0.0f),
	                              glm::radians(45.0f));

	m_window->SwapBuffers();
}

void Game::Initialize(int w, int h)
{
	auto shader = this->resourceManager.CreateShaderProgram("sprite",
	                                                    Shader(ShaderType::Vertex,
	                                                           "../res/shaders/sprite/shader.vert"),
	                                                    Shader(ShaderType::Fragment,
	                                                           "../res/shaders/sprite/shader.frag"));
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(w),
	                                  static_cast<GLfloat>(h), 0.0f,
	                                  -1.0f, 1.0f);
	
	shader->Use();
	shader->SetUniform("projection", projection);
	shader->SetUniform("image", 0);

	this->spriteRenderer.Initialize(shader);
	this->resourceManager.CreateTexture("awesomeFace",
	                                "../res/textures/awesome_face.png",
	                                512, 512, 4, GL_RGBA);
}

void Game::InitializeOpenGL()
{
	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
