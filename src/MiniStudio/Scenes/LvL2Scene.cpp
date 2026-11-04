#include "Scenes/LvL2Scene.h"
#include "Scenes/LvL3Scene.h"
#include "UI/TransitionManager.h"
#include "Graphics/ParallaxManager.h"
#include "Player/Player.h"
#include "Core/GameManager.h"
#include "Audio/AudioMap.h"

void LvL2Scene::OnInitialize()
{
	MyGameScene::OnInitialize();

	ParallaxManager::getInstance().init("res/Background/BackgroundCave");

	InitPlayer();
	ParseMap("res/maps/LvL2.tmx");
	InitCam();
	SetUpUI();

	SetGravity({ 0, 12 });

	TransitionManager::getInstance().SetFadeOut(true);
}

void LvL2Scene::OnWindowEvent(const sf::Event& event)
{

	MyGameScene::OnWindowEvent(event);
}

void LvL2Scene::OnUpdate()
{

	MyGameScene::OnUpdate();
	m_cam->setPosition(m_player->GetPosition() + m_player->getCameraOffSet());
	m_cam->CorrectPosition(sf::Vector2f(m_map.width * m_map.tileWidth, m_map.height * m_map.tileHeight));
	GameManager::Get()->setCamera(*m_cam);

	float pos = m_player->GetPosition(0, 0).x;
	if (pos > m_map.width * m_map.tileWidth && TransitionManager::getInstance().IsFadingIn() == false)
	{
		TransitionManager::getInstance().SetFadeIn(true);
		TransitionManager::getInstance().SetCallBack([this]() { lunchWaitingScreen(); });
		m_player->SetRight(Player::Action::Move, false);
		m_player->SetVelocityY(0);
		m_player->SetAffectedByGravity(false);
		m_player->SetRigidBody(false);
	}

	if (m_loading) {
		m_timer += GetDeltaTime();
		if (m_timer > 3)
			GameManager::Get()->LaunchScene<LvL3Scene>();
	}
}

void LvL2Scene::lunchWaitingScreen()
{
	MyGameScene::lunchWaitingScreen();
}
