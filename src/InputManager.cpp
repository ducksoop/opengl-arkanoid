#include "InputManager.h"
#include "EventHandlers.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <algorithm>

void InputManager::Initialize()
{
}

void InputManager::Close()
{
	m_keyHandlers.clear();
}

void InputManager::ProcessEvents()
{
	glfwPollEvents();
}

void InputManager::ProcessKeyEvent(int key, int scanCode, int action, int mods)
{
	for (const auto& keyHandler : m_keyHandlers)
		keyHandler.second(key, scanCode, action, mods);
}

void InputManager::AddKeyHandler(const std::string& name, KeyHandler keyHandler)
{
	m_keyHandlers[name] = keyHandler;
}

void InputManager::RemoveKeyHandler(const std::string& name)
{
	m_keyHandlers.erase(name);
}
