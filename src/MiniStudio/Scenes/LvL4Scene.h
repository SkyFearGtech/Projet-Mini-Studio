#pragma once
#include "Scenes/MyGameScene.h"

class LvL4Scene : public MyGameScene
{
protected:
	void OnCustomSpawn(const TmxObject& obj) override;

public:
	void OnInitialize() override;
	void OnWindowEvent(const sf::Event& event) override;
	void OnUpdate() override;
};
