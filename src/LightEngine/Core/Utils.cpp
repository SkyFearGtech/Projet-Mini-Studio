#include <cmath>

#include <SFML/System/Vector2.hpp>
#include "Utils.h"
#include "Input/InputManager.h"

namespace Utils 
{
    bool Normalize(sf::Vector2f& vector)
    {
        float magnitude = std::sqrt(vector.x * vector.x + vector.y * vector.y);

		if (magnitude != 0)
		{
			vector.x /= magnitude;
			vector.y /= magnitude;
		
			return true;
		}

		return false;
    }

	float GetDistance(int x1, int y1, int x2, int y2)
	{
		int x = x2 - x1;
		int y = y2 - y1;

		return std::sqrt(x * x + y * y);
	}
	float GetDistance(const sf::Vector2f& v1, const sf::Vector2f& v2)
	{
		sf::Vector2f result = v2 - v1;

		return std::sqrt(result.x * result.y + result.y * result.y);
	}

	float GetDistance(sf::Vector2f a, sf::Vector2f b)
	{
		
		return GetDistance(a.x, a.y, b.x, b.y);
	}

	float GetAngleDegree(const sf::Vector2f& v1, const sf::Vector2f& v2)
	{
		float dot = v1.x * v2.x + v1.y * v2.y;
		float det = v1.x * v2.y - v1.y * v2.x;

		return std::atan2(det, dot) * 180 / 3.14159265;
	}


	//this function was made by IA
	std::string InputToString(const Input& input)
	{
		return std::visit([](auto&& key) -> std::string
		{
			using T = std::decay_t<decltype(key)>;

			if constexpr (std::is_same_v<T, sf::Keyboard::Key>)
			{
				switch (key)
				{
				case sf::Keyboard::A:         return "A";
				case sf::Keyboard::B:         return "B";
				case sf::Keyboard::C:         return "C";
				case sf::Keyboard::D:         return "D";
				case sf::Keyboard::E:         return "E";
				case sf::Keyboard::F:         return "F";
				case sf::Keyboard::G:         return "G";
				case sf::Keyboard::H:         return "H";
				case sf::Keyboard::I:         return "I";
				case sf::Keyboard::J:         return "J";
				case sf::Keyboard::K:         return "K";
				case sf::Keyboard::L:         return "L";
				case sf::Keyboard::M:         return "M";
				case sf::Keyboard::N:         return "N";
				case sf::Keyboard::O:         return "O";
				case sf::Keyboard::P:         return "P";
				case sf::Keyboard::Q:         return "Q";
				case sf::Keyboard::R:         return "R";
				case sf::Keyboard::S:         return "S";
				case sf::Keyboard::T:         return "T";
				case sf::Keyboard::U:         return "U";
				case sf::Keyboard::V:         return "V";
				case sf::Keyboard::W:         return "W";
				case sf::Keyboard::X:         return "X";
				case sf::Keyboard::Y:         return "Y";
				case sf::Keyboard::Z:         return "Z";
				case sf::Keyboard::Num0:      return "0";
				case sf::Keyboard::Num1:      return "1";
				case sf::Keyboard::Num2:      return "2";
				case sf::Keyboard::Num3:      return "3";
				case sf::Keyboard::Num4:      return "4";
				case sf::Keyboard::Num5:      return "5";
				case sf::Keyboard::Num6:      return "6";
				case sf::Keyboard::Num7:      return "7";
				case sf::Keyboard::Num8:      return "8";
				case sf::Keyboard::Num9:      return "9";
				case sf::Keyboard::Escape:    return "Escape";
				case sf::Keyboard::LControl:  return "LCtrl";
				case sf::Keyboard::LShift:    return "LShift";
				case sf::Keyboard::LAlt:      return "LAlt";
				case sf::Keyboard::LSystem:   return "LSystem";
				case sf::Keyboard::RControl:  return "RCtrl";
				case sf::Keyboard::RShift:    return "RShift";
				case sf::Keyboard::RAlt:      return "RAlt";
				case sf::Keyboard::RSystem:   return "RSystem";
				case sf::Keyboard::Menu:      return "Menu";
				case sf::Keyboard::LBracket:  return "[";
				case sf::Keyboard::RBracket:  return "]";
				case sf::Keyboard::Semicolon: return ";";
				case sf::Keyboard::Comma:     return ",";
				case sf::Keyboard::Period:    return ".";
				case sf::Keyboard::Quote:     return "'";
				case sf::Keyboard::Slash:     return "/";
				case sf::Keyboard::Backslash: return "\\";
				case sf::Keyboard::Tilde:     return "~";
				case sf::Keyboard::Equal:     return "=";
				case sf::Keyboard::Hyphen:    return "-";
				case sf::Keyboard::Space:     return "Space";
				case sf::Keyboard::Enter:     return "Enter";
				case sf::Keyboard::Backspace: return "Backspace";
				case sf::Keyboard::Tab:       return "Tab";
				case sf::Keyboard::PageUp:    return "PageUp";
				case sf::Keyboard::PageDown:  return "PageDown";
				case sf::Keyboard::End:       return "End";
				case sf::Keyboard::Home:      return "Home";
				case sf::Keyboard::Insert:    return "Insert";
				case sf::Keyboard::Delete:    return "Delete";
				case sf::Keyboard::Add:       return "Num+";
				case sf::Keyboard::Subtract:  return "Num-";
				case sf::Keyboard::Multiply:  return "Num*";
				case sf::Keyboard::Divide:    return "Num/";
				case sf::Keyboard::Left:      return "Left";
				case sf::Keyboard::Right:     return "Right";
				case sf::Keyboard::Up:        return "Up";
				case sf::Keyboard::Down:      return "Down";
				case sf::Keyboard::Numpad0:   return "Num0";
				case sf::Keyboard::Numpad1:   return "Num1";
				case sf::Keyboard::Numpad2:   return "Num2";
				case sf::Keyboard::Numpad3:   return "Num3";
				case sf::Keyboard::Numpad4:   return "Num4";
				case sf::Keyboard::Numpad5:   return "Num5";
				case sf::Keyboard::Numpad6:   return "Num6";
				case sf::Keyboard::Numpad7:   return "Num7";
				case sf::Keyboard::Numpad8:   return "Num8";
				case sf::Keyboard::Numpad9:   return "Num9";
				case sf::Keyboard::F1:        return "F1";
				case sf::Keyboard::F2:        return "F2";
				case sf::Keyboard::F3:        return "F3";
				case sf::Keyboard::F4:        return "F4";
				case sf::Keyboard::F5:        return "F5";
				case sf::Keyboard::F6:        return "F6";
				case sf::Keyboard::F7:        return "F7";
				case sf::Keyboard::F8:        return "F8";
				case sf::Keyboard::F9:        return "F9";
				case sf::Keyboard::F10:       return "F10";
				case sf::Keyboard::F11:       return "F11";
				case sf::Keyboard::F12:       return "F12";
				case sf::Keyboard::F13:       return "F13";
				case sf::Keyboard::F14:       return "F14";
				case sf::Keyboard::F15:       return "F15";
				case sf::Keyboard::Pause:     return "Pause";
				default:                      return "Unknown";
				}
			}
			else if constexpr (std::is_same_v<T, sf::Mouse::Button>)
			{
				switch (key)
				{
				case sf::Mouse::Left:     return "Mouse Left";
				case sf::Mouse::Right:    return "Mouse Right";
				case sf::Mouse::Middle:   return "Mouse Middle";
				case sf::Mouse::XButton1: return "Mouse X1";
				case sf::Mouse::XButton2: return "Mouse X2";
				default:                  return "Mouse Unknown";
				}
			}
			else // JoystickButton
			{
				switch (key)
				{
				case JoystickButton::Sud:          return "JS Sud";
				case JoystickButton::Est:          return "JS Est";
				case JoystickButton::Ouest:        return "JS Ouest";
				case JoystickButton::Nord:         return "JS Nord";
				case JoystickButton::L:            return "JS L";
				case JoystickButton::R:            return "JS R";
				case JoystickButton::Moin:         return "JS -";
				case JoystickButton::Plus:         return "JS +";
				case JoystickButton::JLeft:        return "JS JLeft";
				case JoystickButton::JRight:       return "JS JRight";
				case JoystickButton::LeftTrigger:  return "JS LT";
				case JoystickButton::RightTrigger: return "JS RT";
				default:                           return "JS Unknown";
				}
			}
		}, input);
	}
}