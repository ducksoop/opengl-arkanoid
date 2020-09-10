#pragma once

#include "Window.h"

#include <memory>
#include <vector>

class WindowManager
{
public:
	static WindowManager& Instance();

	// Copy constructor
	WindowManager(const WindowManager&) = delete;

	// Move constructor
	WindowManager(WindowManager&&) = delete;

	// Copy assignment
	WindowManager& operator=(const WindowManager&) = delete;

	// Move assignment
	WindowManager& operator=(WindowManager&&) = delete;

	void Initialize();
	void Close();

	std::shared_ptr<Window> CreateWindow(int w, int h, const std::string& title, bool fullscreen = false, bool vsync = true);
	
private:
	WindowManager() = default;
	~WindowManager() = default;

	std::vector<std::shared_ptr<Window>> m_windows;
};