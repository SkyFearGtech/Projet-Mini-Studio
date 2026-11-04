#include "UIElem.h"

#include "Core/GameManager.h"
#include "Input/InputManager.h"
#include "Core/Utils.h"


void UIManager::push(UIScreen* _screen)
{
	m_screens.push_back(_screen);
	_screen->OnOpen();
	GameManager::Get()->SetPause(true);
}

void UIManager::pop()
{
	m_screens.back()->OnClose();
	m_screens.pop_back();
	if (m_screens.size() <= 0)
		GameManager::Get()->SetPause(false);

}

void UIManager::update()
{
	if (m_screens.size() == 0)
		return;

	m_screens.back()->AddTime(GameManager::Get()->GetDeltaTime());

	InputManager& im = InputManager::getInstance();

	if (im.isKeyDown(sf::Keyboard::Space) || im.isJoystickButtonDown(JoystickButton::Sud)) {
		m_screens.back()->getCurrentWidget()->OnConfirm();
	}

	if (im.isKeyDown(sf::Keyboard::Escape) || im.isJoystickButtonDown(JoystickButton::Est)) {
		pop();
	}

	sf::Vector2f dir;
	dir.x = im.getAxisValue(JoystickAxis::LeftJoystickX);
	dir.y = im.getAxisValue(JoystickAxis::LeftJoystickY);
	if (abs(dir.x) + abs(dir.y) > 25) {
		Utils::Normalize(dir);
		if (m_screens.back()->getCurrentWidget()->OnDirectional(dir) == false)
			m_screens.back()->navigateTo(dir);

	}
	else {
		if (im.isKeyDown(sf::Keyboard::Up))
			if (m_screens.back()->getCurrentWidget()->OnDirectional(sf::Vector2f(0, -1)) == false)
				m_screens.back()->navigateTo(sf::Vector2f(0, -1));

		if (im.isKeyDown(sf::Keyboard::Down))
			if (m_screens.back()->getCurrentWidget()->OnDirectional(sf::Vector2f(0, 1)) == false)
				m_screens.back()->navigateTo(sf::Vector2f(0, 1));

		if (im.isKeyDown(sf::Keyboard::Left))
			if (m_screens.back()->getCurrentWidget()->OnDirectional(sf::Vector2f(-1, 0)) == false)
				m_screens.back()->navigateTo(sf::Vector2f(-1, 0));

		if (im.isKeyDown(sf::Keyboard::Right))
			if (m_screens.back()->getCurrentWidget()->OnDirectional(sf::Vector2f(1, 0)) == false)
				m_screens.back()->navigateTo(sf::Vector2f(1, 0));

	}
}

void UIManager::render(sf::RenderTarget& _surface)
{

	if (m_screens.size() == 0)
		return;

	sf::View savedView = _surface.getView();
	_surface.setView(_surface.getDefaultView());

	m_screens.back()->OnDraw(_surface);

	_surface.setView(savedView);


}

void UIScreen::navigateTo(sf::Vector2f  _dir)
{
	if (m_lastChange < m_changeInter) return;


	if (UIwidget* wid = findNearest(_dir)) {
		m_currentWidget->OnUnFocus();
		m_currentWidget = wid;
		m_currentWidget->OnFocus();
		m_lastChange = 0.f;
	}
}

void UIScreen::addWidget(UIwidget* _widget)
{
	m_widgets.push_back(_widget);
}

UIwidget* UIScreen::findNearest(sf::Vector2f _dir)
{

	sf::Vector2f focusPos = m_currentWidget->getPos();


	float minDist = INT_MAX;
	UIwidget* fav = nullptr;

	for (UIwidget* widget : m_widgets) {

		if (widget == m_currentWidget) continue;
		if (widget->isFocusable() == false) continue;

		sf::Vector2f candidatePos = widget->getPos();
		sf::Vector2f delta = candidatePos - focusPos;

		if (dot(delta, _dir) < 0.3) continue;

		if (float dist = Utils::GetDistance(focusPos, candidatePos); dist < minDist) {
			minDist = dist;
			fav = widget;
		}
	}
	return fav;
}

float UIScreen::dot(sf::Vector2f a, sf::Vector2f b) const
{
	return a.x * b.x + a.y * b.y;
}
