#pragma once
#include "UI/UIElem.h"


class PauseScreen : public UIScreen {

public:
	void OnOpen() override;
	void OnClose() override;
	void OnDraw(sf::RenderTarget& _surface) override;

private:
	void Resume();
	void Quit();
	void SetVolume(float _vol);
	void ShowDebug(bool state);
};