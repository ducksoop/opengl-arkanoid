#pragma once

#include <functional>

using KeyHandler = std::function<void(int key, int scanCode, int action, int mods)>;