#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Singleton.h"
#include "WindowManager.h"
#include "Window.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Game.h"

WindowManager& windowManager = WindowManager::Instance();
InputManager& inputManager = InputManager::Instance();
ResourceManager& resourceManager = ResourceManager::Instance();

const int WIDTH = 800;
const int HEIGHT = 600;

int main(int argc, char* argv[])
{
	windowManager.Initialize();
	inputManager.Initialize();
	resourceManager.Initialize();
	
	auto window = windowManager.CreateWindow(WIDTH, HEIGHT, "Arkanoid");

	inputManager.AddKeyHandler("exit", [&window](int key, int scanCode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			window->SetShouldClose(true);
	});
	
	Game game(window);

	GLfloat delta;
	GLfloat lastFrame = static_cast<GLfloat>(glfwGetTime());

	while (!window->IsClosing())
	{
		// Calculate delta time
		GLfloat currentFrame = static_cast<GLfloat>(glfwGetTime());
		delta = currentFrame - lastFrame;
		lastFrame = currentFrame;

		game.HandleInput(delta);
		game.Update(delta);
		game.Render();
	}

	resourceManager.Close();
	inputManager.Close();
	windowManager.Close();

	return EXIT_SUCCESS;
}
