#include "WindowManager.h"

#include <iostream>

WindowManager& WindowManager::Instance()
{
	static WindowManager instance;
	return instance;
}

void WindowManager::Initialize()
{
	// Setup error handling
	glfwSetErrorCallback([](int errorCode, const char* description) -> void
	{
		throw std::runtime_error(description);
	});

	if (!glfwInit())
		std::cerr << "Failed to initialize GLFW" << std::endl;

	glfwDefaultWindowHints();

	// Using OpenGL 3.3 Core Profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

void WindowManager::Close()
{
	for (auto window : m_windows)
		window->Destroy();

	glfwTerminate();
}

std::shared_ptr<Window> WindowManager::CreateWindow(int w, int h, const std::string& title, bool fullscreen, bool vsync)
{
	std::shared_ptr<Window> window(new Window(w, h, title, fullscreen, vsync));
	m_windows.push_back(window);
	
	return window;
}
