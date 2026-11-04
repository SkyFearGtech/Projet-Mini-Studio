#pragma once

#include <variant>
#include <vector>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include "Input/InputManager.h"

enum class TriggerState { Down, Held, Released };

using Input = std::variant<sf::Keyboard::Key, sf::Mouse::Button, JoystickButton>;

struct KeyBinding {
    std::vector<std::pair<Input, TriggerState>> inputs;
};