#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>
#include <variant>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include "Input/KeysBinding.h"

namespace Utils
{
	bool Normalize(sf::Vector2f& vector);
	float GetDistance(int x1, int y1, int x2, int y2);
	float GetDistance(const sf::Vector2f& v1,const sf::Vector2f& v2);
	float GetAngleDegree(const sf::Vector2f& v1, const sf::Vector2f& v2);

	std::string InputToString(const Input& input);
}