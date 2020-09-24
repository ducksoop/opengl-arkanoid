#include "Window.h"
#include "InputManager.h"

#include <iostream>

Window::Window(int width, int height, const std::string& title, bool fullscreen, bool vsync)
	: m_width(width)
	, m_height(height)
	, m_title(title)
	, m_isFullScreen(fullscreen)
	, m_vsync(vsync)
{
	auto primaryMonitor = glfwGetPrimaryMonitor();
	m_window = fullscreen ? glfwCreateWindow(width, height, title.c_str(), primaryMonitor, nullptr)
						  : glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

	if (m_window == nullptr)
		std::cerr << "Failed to create GLFW window!\n";

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	if (!fullscreen)
	{
		// Center the window on screen
		auto videoMode = glfwGetVideoMode(primaryMonitor);
		glfwSetWindowPos(m_window, (videoMode->width - width) / 2, (videoMode->height - height) / 2);
	}

	// Create OpenGL context
	MakeContextCurrent();

	// Set Vsync
	if (vsync)
		glfwSwapInterval(1);

	// Define the viewport dimensions
	int frameBufferWidth;
	int frameBufferHeight;
	glfwGetFramebufferSize(m_window, &frameBufferWidth, &frameBufferHeight);
	glViewport(0, 0, frameBufferWidth, frameBufferHeight);

	SetupEventHandlers();
}

bool Window::IsClosing() const
{
	return static_cast<bool>(glfwWindowShouldClose(m_window));
}

void Window::SetShouldClose(bool flag) const
{
	glfwSetWindowShouldClose(m_window, flag);
}

void Window::MakeContextCurrent() const
{
	glfwMakeContextCurrent(m_window);
}

void Window::SwapBuffers() const
{
	glfwSwapBuffers(m_window);
}

int Window::GetWidth() const
{
	return m_width;
}

int Window::GetHeight() const
{
	return m_height;
}

void Window::Destroy() const
{
	glfwDestroyWindow(m_window);
}

void Window::SetupEventHandlers() const
{
	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scanCode, int action, int mods)
	{
		InputManager::Instance().ProcessKeyEvent(key, scanCode, action, mods);
	});
}
