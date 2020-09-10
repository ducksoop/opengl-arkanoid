#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "WindowManager.h"
#include "Window.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "ShaderType.h"
#include "Texture.h"
#include "ResourceManager.h"

#include <iostream>
#include <cstdlib>

WindowManager& windowManager = WindowManager::Instance();
ResourceManager& resourceManager = ResourceManager::Instance();

const int WIDTH = 800;
const int HEIGHT = 600;

int main(int argc, char* argv[])
{
	windowManager.Initialize();
	resourceManager.Initialize();
	
	auto window = windowManager.CreateWindow(WIDTH, HEIGHT, "Arkanoid");
	
	glewExperimental = GL_TRUE;
	glewInit();

	// Creating a triangle
	GLfloat vertices[] = {
		// Positions	// Texture coordinates
		0.5f, 0.5f,		1.0f, 1.0f,		// Top right
		0.5f, -0.5f,	1.0f, 0.0f,		// Bottom right
		-0.5f, -0.5f,	0.0f, 0.0f,		// Bottom left
		-0.5f, 0.5f,	0.0f, 1.0f		// Top left
	};

	GLuint indices[] = {
		0, 1, 3,	// First triangle
		1, 2, 3		// Second triangle
	};
	
	// Buffer setup
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	auto shader = resourceManager.CreateShaderProgram("basic",
		Shader(ShaderType::Vertex,
			"../res/shaders/basic/shader.vert"),
		Shader(ShaderType::Fragment,
			"../res/shaders/basic/shader.frag"));

	GLuint textureWidth = 512;
	GLuint textureHeight = 512;
	
	auto woodenContainer = resourceManager.CreateTexture("woodenContainer",
		"../res/textures/wooden_container.jpg",
		textureWidth,
		textureHeight);
	auto awesomeFace = resourceManager.CreateTexture("awesomeFace",
		"../res/textures/awesome_face.png",
		textureWidth,
		textureHeight,
		4,
		GL_RGBA);


	while (!window->IsClosing())
	{
		glfwPollEvents();

		glClearColor(0.1f, 0.1f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader->Use();

		woodenContainer->Bind(0);
		shader->SetUniform("texture1", 0);
		awesomeFace->Bind(1);
		shader->SetUniform("texture2", 1);
		
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		window->SwapBuffers();
	}

	// Clean up all the resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	resourceManager.Close();
	windowManager.Close();

	return EXIT_SUCCESS;
}
