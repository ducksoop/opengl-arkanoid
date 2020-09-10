#pragma once

#include <GLFW/glfw3.h>
#include <string>

class Window
{
public:
	~Window() = default;

	bool IsClosing() const;
	void SetShouldClose(bool flag);
	void MakeContextCurrent();
	void SwapBuffers();
	
private:
	Window(int width, int height, const std::string& title, bool fullscreen, bool vsync);

	void Destroy();

	friend class WindowManager;
	
	int m_width;
	int m_height;
	std::string m_title;
	bool m_isFullScreen;
	bool m_vsync;
	GLFWwindow* m_window;
};
