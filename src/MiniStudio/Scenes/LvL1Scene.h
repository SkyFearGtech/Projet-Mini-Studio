#pragma once
#include "Scenes/MyGameScene.h"

class Player;
class Rock;
class QTEEntity;

class LvL1Scene : public MyGameScene
{

private:

	Rock* m_rock;
	QTEEntity* m_qte;

	void InitPlayer();
	void InitCam();

	void lunchWaitingScreen();

public:
	void OnInitialize() override;
	void OnUpdate() override;


	void OnWindowEvent(const sf::Event& event) override;

};

