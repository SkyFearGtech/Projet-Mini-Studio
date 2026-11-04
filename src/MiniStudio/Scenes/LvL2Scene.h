#pragma once
#include "Scenes/MyGameScene.h"

class LvL2Scene : public MyGameScene
{
private:
	void lunchWaitingScreen();

public:
	void OnInitialize() override;
	void OnWindowEvent(const sf::Event& event) override;
	void OnUpdate() override;
};
