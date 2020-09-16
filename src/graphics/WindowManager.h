#pragma once

#include "Singleton.h"
#include "Window.h"

#include <memory>
#include <vector>

class WindowManager : public Singleton<WindowManager>
{
public:
	void Initialize();
	void Close();

	Window* CreateWindow(int w, int h, const std::string& title, bool fullscreen = false, bool vsync = true);
	
private:
	WindowManager() = default;
	~WindowManager() = default;

	std::vector<std::unique_ptr<Window>> m_windows;

	friend Singleton;
};