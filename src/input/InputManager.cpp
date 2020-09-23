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

void InputManager::ProcessEvents(float dt)
{
	m_delta = dt;
	glfwPollEvents();

	for (const auto& keyHandler : m_keyHandlers)
		keyHandler.second(m_delta);
}

void InputManager::ProcessKeyEvent(int key, int scanCode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		m_keys[key] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		m_keys[key] = false;
		m_processedKeys[key] = false;
	}
}

void InputManager::AddKeyHandler(const std::string& name, KeyHandler keyHandler)
{
	m_keyHandlers[name] = keyHandler;
}

void InputManager::RemoveKeyHandler(const std::string& name)
{
	m_keyHandlers.erase(name);
}

bool InputManager::IsKeyPressed(int key)
{
	return m_keys[key];
}

bool InputManager::IsKeyProcessed(int key)
{
	return m_processedKeys[key];
}

void InputManager::SetProcessedKey(int key)
{
	m_processedKeys[key] = true;
}
