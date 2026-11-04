#include "UIElem.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <functional>
#include <string>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Graphics/Texture.h"
#include "SFML/Graphics/Sprite.hpp"

#include <variant>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "Input/KeysBinding.h"

#include "Core/Utils.h"

class UILabel : public UIwidget {

private:
	sf::Text m_text;

public:
	void OnFocus() override {};
	void OnUnFocus() override {};
	void OnConfirm() override {};
	void OnDraw(sf::RenderTarget& _surface) override;
	bool isFocusable() const { return false; }

	UILabel(sf::Vector2f _pos, sf::Vector2f _size, std::string _text, const sf::Font& _font, sf::Color _color) : UIwidget(_pos, _size), m_text(_text, _font)
	{
		m_text.setPosition(_pos); 
		m_text.setFillColor(_color);
	}
};


class UIButton : public UIwidget {

private:
	sf::Text m_text;
	std::function<void()> m_callBack;

	sf::Texture m_defaultTexture;
	sf::Texture m_focusTexture;

	sf::Sprite m_sprite;

public:
	void OnFocus() override;
	void OnUnFocus() override;
	void OnConfirm() override;
	void OnDraw(sf::RenderTarget& _surface) override;

	UIButton(sf::Vector2f _pos, sf::Vector2f _size, std::function<void()> _func, std::shared_ptr<Texture> _defaultTexture = nullptr, std::shared_ptr<Texture> _focusTexture = nullptr)
		: UIwidget(_pos, _size),
		m_callBack(_func)
	{
		if (_defaultTexture) 
			m_defaultTexture = *_defaultTexture->getTexture();
		
		if (_focusTexture)
			m_focusTexture = *_focusTexture->getTexture();

		m_sprite = sf::Sprite(m_defaultTexture);
		m_sprite.setPosition(_pos);
		if (_defaultTexture)
			m_sprite.setScale(sf::Vector2f(_size.x / m_sprite.getTexture()->getSize().x, _size.y / m_sprite.getTexture()->getSize().y));
	}

	UIButton(sf::Vector2f _pos, sf::Vector2f _size, std::function<void()> _func, std::string _text, const sf::Font& _font,
		std::shared_ptr<Texture> _defaultTexture = nullptr, std::shared_ptr<Texture> _focusTexture = nullptr)
		: UIwidget(_pos, _size),
		m_callBack(_func),
		m_text(_text, _font)
	{
		if (_defaultTexture) 
			m_defaultTexture = *_defaultTexture->getTexture();
		
		if (_focusTexture)
			m_focusTexture = *_focusTexture->getTexture();

			m_sprite = sf::Sprite(m_defaultTexture);
		m_sprite.setPosition(_pos);
		if (_defaultTexture)
			m_sprite.setScale(sf::Vector2f(_size.x / m_sprite.getTexture()->getSize().x, _size.y / m_sprite.getTexture()->getSize().y));


		sf::Vector2f textPos = _pos;
		textPos.y += _size.y / 2 - m_text.getGlobalBounds().height;

		m_text.setPosition(textPos);
		m_text.setFillColor(sf::Color(129, 135, 145));
	}
};

class UICheckBox : public UIwidget {

private:

	bool m_state;
	std::function<void(bool)> m_callBack;

	sf::Text m_text;

	sf::Texture m_emptyTexture;
	sf::Texture m_fullTexture;

	sf::Sprite m_sprite;


public:
	void OnFocus() override;
	void OnUnFocus() override;
	void OnConfirm() override;
	void OnDraw(sf::RenderTarget& _surface) override;

	UICheckBox(sf::Vector2f _pos, sf::Vector2f _size, std::function<void(bool)> _func, bool _state, std::string _text, const sf::Font& _font,
		std::shared_ptr<Texture> _emptyTexture = nullptr, std::shared_ptr<Texture> _fullTexture = nullptr)
		: UIwidget(_pos, _size),
		m_callBack(_func),
		m_state(_state),
		m_text(_text, _font)
	{
		if (_emptyTexture)
		
			m_emptyTexture = *_emptyTexture->getTexture();
		
		if (_fullTexture)
			m_fullTexture = *_fullTexture->getTexture();

		m_sprite = sf::Sprite(_state ? m_fullTexture : m_emptyTexture);
		m_sprite.setPosition(_pos);
		if (_emptyTexture || _fullTexture)
			m_sprite.setScale(sf::Vector2f(_size.x / m_sprite.getTexture()->getSize().x, _size.y / m_sprite.getTexture()->getSize().y));
		//on centre le texte apres la boite
		sf::Vector2f textPos = _pos;
		textPos.y += _size.y / 2 - m_text.getGlobalBounds().height / 2;
		textPos.x += _size.x * 1.5;

		m_text.setPosition(textPos);
		m_text.setFillColor(sf::Color(129, 135, 145));
	}
};

class UISlider : public UIwidget {

private:
	bool m_secure;
	bool m_confirm = false;

	bool  m_horizontal;

	float m_min;
	float m_max;
	float m_value;
	float m_step;

	std::function<void(float)> m_callBack;

	sf::Text m_info;

	sf::Sprite m_track;
	sf::Sprite m_knob;

	bool m_trackHasTexture = false;
	bool m_knobHasTexture = false;


	void OnFocus() override;
	void OnUnFocus() override;
	void OnConfirm() override;
	void OnDraw(sf::RenderTarget& _surface) override;

	bool OnDirectional(sf::Vector2f _direction) override;


public:
	UISlider(sf::Vector2f _pos, sf::Vector2f _size, std::function<void(float)> _func, float _min, float _max, float _value, float _step, bool _secure,
		std::shared_ptr<Texture> _trackTexture = nullptr, std::shared_ptr<Texture> _knobTexture = nullptr)
		: UIwidget(_pos, _size),
		m_callBack(_func),
		m_min(_min),
		m_max(_max),
		m_value(_value),
		m_step(_step),
		m_secure(_secure)
	{
		m_horizontal = _size.x > _size.y;
	
		if (_trackTexture) {
			m_track.setTexture(*_trackTexture->getTexture());
			m_track.setScale(sf::Vector2f(_size.x / m_track.getTexture()->getSize().x, _size.y / m_track.getTexture()->getSize().y));
			m_trackHasTexture = true;
		}
		if (_knobTexture) {
			m_knob.setTexture(*_knobTexture->getTexture());
			m_knob.setScale(sf::Vector2f(_size.x / m_track.getTexture()->getSize().x / 10, _size.y / m_track.getTexture()->getSize().y));
			m_knobHasTexture = true;
		}
	}
	UISlider(sf::Vector2f _pos, sf::Vector2f _size, std::function<void(float)> _func, float _min, float _max, float _value, float _step, bool _secure,
		std::string _text, const sf::Font& _font,
		std::shared_ptr<Texture> _trackTexture = nullptr, std::shared_ptr<Texture> _knobTexture = nullptr)
		: UIwidget(_pos, _size),
		m_callBack(_func),
		m_min(_min),
		m_max(_max),
		m_value(_value),
		m_step(_step),
		m_secure(_secure),
		m_info(_text, _font)
	{
		m_horizontal = _size.x > _size.y;

		if (_trackTexture) {
			m_track.setTexture(*_trackTexture->getTexture());
			m_track.setScale(sf::Vector2f(_size.x / m_track.getTexture()->getSize().x, _size.y / m_track.getTexture()->getSize().y));
			m_trackHasTexture = true;
		}
		if (_knobTexture) {
			m_knob.setTexture(*_knobTexture->getTexture());
			m_knob.setScale(sf::Vector2f(_size.x / m_track.getTexture()->getSize().x / 10, _size.y / m_track.getTexture()->getSize().y));
			m_knobHasTexture = true;
		}



		sf::Vector2f infoPos = _pos;

		infoPos.x += _size.x / 2;
		infoPos.x -= m_info.getGlobalBounds().width / 2;
		infoPos.y -= m_info.getGlobalBounds().height * 1.5f;
		m_info.setPosition(infoPos);
		m_info.setFillColor(sf::Color(129, 135, 145));
	}
};

class UIKeyBind : public UIwidget {

private:
	std::string m_actionName;
	sf::Text m_display;
	Input m_currentKey;
	bool m_capturing;

	std::function<void(Input)> m_callBack;

public:
	void OnFocus() override;
	void OnUnFocus() override;
	void OnConfirm() override;
	void OnDraw(sf::RenderTarget& _surface) override;

	bool OnDirectional(sf::Vector2f _direction) override;

	UIKeyBind(sf::Vector2f _pos, sf::Vector2f _size, std::function<void(Input)> _func, Input _currentKey, std::string _actionName, const sf::Font& _font)
		: UIwidget(_pos, _size),
		m_callBack(_func),
		m_currentKey(_currentKey),
		m_actionName(_actionName),
		m_capturing(false)
	{
		std::string fullString = m_actionName + ": [" + Utils::InputToString(m_currentKey) + ']';
		m_display = sf::Text(fullString, _font);	
		m_display.setPosition(_pos);
	}
};