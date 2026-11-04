#include "UIWidgetBasic.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>

void UILabel::OnDraw(sf::RenderTarget& _surface)
{
	_surface.draw(m_text);
}

void UIButton::OnFocus()
{
	m_focused = true;
	m_sprite.setTexture(m_focusTexture);
}

void UIButton::OnUnFocus()
{
	m_focused = false;
	m_sprite.setTexture(m_defaultTexture);
}

void UIButton::OnConfirm()
{
	m_callBack();
}

void UIButton::OnDraw(sf::RenderTarget& _surface)
{
	//draw de la texture
	if (m_sprite.getTexture()->getSize() != sf::Vector2u(0, 0))
		_surface.draw(m_sprite);
	else {
		//sinon carre bleu ou blanc
		sf::RectangleShape rect(sf::Vector2f(m_size.x, m_size.y));
		rect.setPosition(m_position.x, m_position.y);
		rect.setFillColor( m_focused ? sf::Color::Blue: sf::Color::White);
		_surface.draw(rect);
	}
	//draw du text
	if (m_text.getString() != "")
		_surface.draw(m_text);
}

void UICheckBox::OnFocus()
{
	m_focused = true;
}

void UICheckBox::OnUnFocus()
{
	m_focused = false;
}

void UICheckBox::OnConfirm()
{
	m_state = !m_state;
	m_callBack(m_state);
	m_sprite.setTexture(m_state ? m_fullTexture: m_emptyTexture);
}

void UICheckBox::OnDraw(sf::RenderTarget& _surface)
{

	//draw de la texture
	if (m_sprite.getTexture()->getSize() != sf::Vector2u(0, 0))
		_surface.draw(m_sprite);
	else {
		//sinon, carre blanc ou bleu
		sf::RectangleShape rect(sf::Vector2f(m_size.x, m_size.y));
		rect.setPosition(m_position.x, m_position.y);
		rect.setFillColor(m_focused ? sf::Color::Blue : sf::Color::White);
		_surface.draw(rect);

		//avec un croix si jamais on est bon
		if (m_state) {
			sf::VertexArray line(sf::Lines, 2);

			line[0].color = sf::Color::Black;
			line[1].color = sf::Color::Black;


			line[0].position = sf::Vector2f(m_position.x, m_position.y);
			line[1].position = sf::Vector2f(m_position.x + m_size.x, m_position.y + m_size.y);
			_surface.draw(line);
			
			line[0].position = sf::Vector2f(m_position.x + m_size.x, m_position.y);
			line[1].position = sf::Vector2f(m_position.x, m_position.y + m_size.y);
			_surface.draw(line);
		}
	}

	//draw du text
	_surface.draw(m_text);
}

void UISlider::OnFocus()
{
	m_focused = true;
}

void UISlider::OnUnFocus()
{
	m_focused = false;
}

void UISlider::OnConfirm()
{
	if (m_secure)
		m_confirm = !m_confirm;
}

void UISlider::OnDraw(sf::RenderTarget& _surface)
{

	if (m_trackHasTexture)
		_surface.draw(m_track);
	else
	{
		sf::RectangleShape rect(sf::Vector2f(m_size.x, m_size.y));
		rect.setPosition(m_position.x, m_position.y);
		rect.setFillColor(sf::Color(129, 135, 145));
		_surface.draw(rect);
	}

	if (m_knobHasTexture)
		_surface.draw(m_knob);
	else {

		sf::RectangleShape rect(sf::Vector2f(m_size.x / 10, m_size.y));

		float coef = (m_value - m_min) / (m_max - m_min);
		rect.setPosition(m_position.x + (coef * m_size.x) * (int)(m_horizontal), m_position.y + (coef * m_size.y) * (int)(!m_horizontal));
		rect.setFillColor(m_focused ? m_secure && !m_confirm  ? sf::Color::White : sf::Color::Blue : sf::Color::White);
		_surface.draw(rect);
	}

	if (m_info.getString() != "") {
		_surface.draw(m_info);
	}

}

bool UISlider::OnDirectional(sf::Vector2f _direction)
{
	if (m_secure && m_confirm == false) 

		return false;
	

	//si on est pas dans le bon sens, on skip
	if ((abs(_direction.x) > abs(_direction.y)) != m_horizontal) 
		return false;
	
	
	float value = m_horizontal ? _direction.x : _direction.y;

	if (value > 0)
		m_value += m_step;
	else
		m_value -= m_step;


	m_value = std::clamp(m_value, m_min, m_max);
	m_callBack(m_value);

	return true;
}

void UIKeyBind::OnFocus()
{
}

void UIKeyBind::OnUnFocus()
{
}

void UIKeyBind::OnConfirm()
{
	m_capturing = true;
	std::string fullString = m_actionName + ": [...]";
	m_display.setString(fullString);
	InputManager::getInstance().setNextInputCallBack([this](Input key) {
		m_currentKey = key;
		m_capturing = false;
		m_callBack(key);
		std::string fullString = m_actionName + ": [" + Utils::InputToString(key) + ']';
		m_display.setString(fullString);
		});
}

void UIKeyBind::OnDraw(sf::RenderTarget& _surface)
{
	_surface.draw(m_display);
}

bool UIKeyBind::OnDirectional(sf::Vector2f _direction)
{
	return m_capturing;
}
