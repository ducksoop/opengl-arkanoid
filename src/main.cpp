#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Game.h"

const int WIDTH = 1920;
const int HEIGHT = 1080;
const bool FULLSCREEN_ON = true;

int main(int argc, char* argv[])
{
	Game game(WIDTH, HEIGHT, FULLSCREEN_ON);

	GLfloat delta;
	GLfloat lastFrame = static_cast<GLfloat>(glfwGetTime());

	while (!game.IsExiting())
	{
		// Calculate delta time
		GLfloat currentFrame = static_cast<GLfloat>(glfwGetTime());
		delta = currentFrame - lastFrame;
		lastFrame = currentFrame;

		game.HandleInput(delta);
		game.Update(delta);
		game.Render();
	}

	return EXIT_SUCCESS;
}
