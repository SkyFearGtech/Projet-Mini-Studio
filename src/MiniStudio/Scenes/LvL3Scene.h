#pragma once
#include "Scenes/MyGameScene.h"

class LvL3Scene : public MyGameScene
{
public:
	void OnInitialize() override;
	void OnWindowEvent(const sf::Event& event) override;
	void OnUpdate() override;
};
