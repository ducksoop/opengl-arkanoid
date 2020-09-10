#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "WindowManager.h"
#include "Window.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "ShaderType.h"
#include "FileManager.h"

#include <iostream>
#include <cstdlib>

WindowManager& windowManager = WindowManager::Instance();
FileManager& fileManager = FileManager::Instance();

const int WIDTH = 800;
const int HEIGHT = 600;

int main(int argc, char* argv[])
{
	windowManager.Initialize();
	auto window = windowManager.CreateWindow(WIDTH, HEIGHT, "Arkanoid");
	
	glewExperimental = GL_TRUE;
	glewInit();

	// Creating a triangle
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};
	
	// Buffer setup
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	ShaderProgram shader(
		Shader(ShaderType::Vertex, fileManager.ReadAsText("../res/shaders/basic/shader.vert")),
		Shader(ShaderType::Fragment, fileManager.ReadAsText("../res/shaders/basic/shader.frag"))
	);

	while (!window->IsClosing())
	{
		glfwPollEvents();

		glClearColor(0.1f, 0.1f, 0.1f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw triangle
		shader.Use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		window->SwapBuffers();
	}

	// Clean up all the resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	windowManager.Close();

	return EXIT_SUCCESS;
}
