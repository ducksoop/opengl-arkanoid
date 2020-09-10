#pragma once

#include "EventHandlers.h"

#include <map>
#include <string>

class InputManager
{
public:
    static InputManager& Instance();
	
    // Copy constructor
    InputManager(const InputManager&) = delete;
	
    // Move constructor
    InputManager(const InputManager&&) = delete;
	
    // Copy assignment
    InputManager& operator =(const InputManager&) = delete;
	
    // Move assignment
    InputManager& operator =(const InputManager&&) = delete;

    void Initialize();
    void Close();

    void ProcessEvents();
    void ProcessKeyEvent(int key, int scanCode, int action, int mods);

    void AddKeyHandler(const std::string& name, KeyHandler keyHandler);
    void RemoveKeyHandler(const std::string& name);
	
private:
    InputManager() = default;
	~InputManager() = default;

	std::map<std::string, KeyHandler> m_keyHandlers;
};