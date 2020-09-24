#pragma once

#include <GLFW/glfw3.h>
#include <string>

class Window
{
public:
	~Window() = default;

	bool IsClosing() const;
	void SetShouldClose(bool flag) const;
	void MakeContextCurrent() const;
	void SwapBuffers() const;

	int GetWidth() const;
	int GetHeight() const;
	
private:
	Window(int width, int height, const std::string& title, bool fullscreen, bool vsync);

	void Destroy() const;
	void SetupEventHandlers() const;

	int m_width;
	int m_height;
	std::string m_title;
	bool m_isFullScreen;
	bool m_vsync;
	GLFWwindow* m_window;

	friend class WindowManager;
};
