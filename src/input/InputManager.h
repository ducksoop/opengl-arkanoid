#pragma once

#include "Singleton.h"
#include "EventHandlers.h"

#include <map>
#include <string>

class InputManager : public Singleton<InputManager>
{
public:
    void Initialize();
    void Close();

    void ProcessEvents(float dt);
    void ProcessKeyEvent(int key, int scanCode, int action, int mods);

    void AddKeyHandler(const std::string& name, KeyHandler keyHandler);
    void RemoveKeyHandler(const std::string& name);

    bool IsKeyPressed(int key);
	
private:
    InputManager() = default;
	~InputManager() = default;

	std::map<std::string, KeyHandler> m_keyHandlers;

    float m_delta;
    bool m_keys[348]; // GLFW3 defines 348 different key codes

    friend Singleton;
};