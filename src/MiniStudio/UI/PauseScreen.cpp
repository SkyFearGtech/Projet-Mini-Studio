#include "PauseScreen.h"
#include "UI/UIWidgetBasic.h"
#include "Core/GameManager.h"

#include "Core/Debug.h"

#include "Core/Scene.h"

void PauseScreen::OnOpen()
{

	UIButton* resume = new UIButton(sf::Vector2f(150, 150), sf::Vector2f(150, 40), [this] {Resume();}, "Resume", GameManager::Get()->GetFont());
	UIButton* quit = new UIButton(sf::Vector2f(150, 200), sf::Vector2f(150, 40), [this] {Quit();}, "Quit", GameManager::Get()->GetFont());

	UISlider* volume = new UISlider(sf::Vector2f(150, 300), sf::Vector2f(500, 40), [this](float volume) {SetVolume(volume);}, 0, 1, 0.5, 0.01, false, "Volume générale", GameManager::Get()->GetFont());


	bool state = GameManager::Get()->ShowDebug();
	UICheckBox* showDebug = new UICheckBox(sf::Vector2f(150, 370), sf::Vector2f(40, 40), [this](bool state) {ShowDebug(state);}, state,  "Show Debug", GameManager::Get()->GetFont());

	addWidget(resume);
	addWidget(quit);
	addWidget(volume);
	addWidget(showDebug);

	m_currentWidget = m_widgets[0];
	m_currentWidget->OnFocus();

}

void PauseScreen::OnClose()
{
}

void PauseScreen::OnDraw(sf::RenderTarget& _surface)
{
	for (UIwidget* w : m_widgets)
		w->OnDraw(_surface);

}

void PauseScreen::Resume()
{
	UIManager::getInstance().pop();
}

void PauseScreen::Quit()
{
	GameManager::Get()->GetWindow()->close();
}

void PauseScreen::SetVolume(float _vol)
{
	GameManager::Get()->GetScene()->getAudioMap()->setGeneraleVolume(_vol);
}

void PauseScreen::ShowDebug(bool state)
{
	GameManager::Get()->ShowDebug(state);
}
