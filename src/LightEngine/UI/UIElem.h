#pragma once

#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include <vector>


class UIwidget {
protected:
	sf::Vector2f m_position;
	sf::Vector2f m_size;
	bool m_focused;


public:
	
	UIwidget(sf::Vector2f _pos, sf::Vector2f _size) : m_position(_pos), m_size(_size), m_focused(false) {}

	virtual void OnFocus() = 0;
	virtual void OnUnFocus() = 0;
	virtual void OnConfirm() = 0;
	virtual void OnDraw(sf::RenderTarget& _surface) = 0;

	virtual bool OnDirectional(sf::Vector2f _direction) { return false; }
	virtual bool isFocusable() const { return true; }

	sf::Vector2f getPos() const { return m_position; }


};


class UIScreen {
protected:
	std::vector<UIwidget*> m_widgets;
	UIwidget* m_currentWidget;
	float m_lastChange = 0;
	float m_changeInter = 0.3;

public:

	UIwidget* getCurrentWidget() const { return m_currentWidget; }
    
	virtual void OnOpen() = 0;
	virtual void OnClose() = 0;
	virtual void OnDraw(sf::RenderTarget& _surface) = 0;

	void navigateTo(sf::Vector2f  _direction);
	void addWidget(UIwidget* _widget);

	void AddTime(float _dt) { m_lastChange += _dt; }

	virtual ~UIScreen() {
		for (UIwidget* w : m_widgets) {
			delete w;
		}
	}
private:
	UIwidget* findNearest(sf::Vector2f  _dir);
	float dot(sf::Vector2f a, sf::Vector2f b) const;
};


class UIManager {

private:

	std::vector<UIScreen*> m_screens;



	UIManager() = default;
	UIManager(const UIManager&) = delete;
	UIManager& operator=(const UIManager&) = delete;

public:

	static UIManager& getInstance() {
		static UIManager instance;
		return instance;
	}
	
	void push(UIScreen* _screen);
	void pop();

	void update();
	void render(sf::RenderTarget& _surface);

};