#include "LvL1Scene.h"
#include "LvL2Scene.h"
#include "Entity/Wall.h"
#include "Player/Player.h"

#include "Assets/AssetsManager.h"
#include "Graphics/LightManager.h"
#include <random>
#include "Entity/POPEntity.h"
#include "UI/TransitionManager.h"
#include "Graphics/ParallaxManager.h"
#include "Core/Debug.h"
#include "Entity/Stele.h"

#include <random>

#include "Enemies/Enemies.h"

#include "Map/TmxThing.h"

#include "Entity/Rock.h"
#include "Entity/QTEEntity.h"

#include "Audio/AudioMap.h"
#include "UI/PauseScreen.h"
#include "UI/UIElem.h"


void LvL1Scene::InitPlayer()
{
	m_player = CreateEntity<Player>({ 80, 150 }, sf::Color::White);
	m_player->init(6);
	
	m_player->SetTexture(AssetsManager::getInstance().getTexture("AssetsPlayer", "res/Player/playerFinal.png"));
	m_player->SetRigidBody(true);
	m_player->setSpeed(2500);
	m_player->SetMaxRunSpeed(700);
	m_player->SetMaxFallSpeed(800);

	m_player->GetAnimator().loadFromJson("res/JsonAnimation/playerFinal.json");
	m_player->SetSpriteOffset({ -135.f, -100.f });
	m_player->SetSpriteOffsetFlipped({ -135, -100.f });

	m_player->GetAnimator().applyScale(m_player->GetSprite(), { 350, 340 });
	m_player->GetAnimator().play("Idle");


	m_rock = CreateEntity<Rock>(5, sf::Color::Transparent);
	m_rock->init(7);
	m_rock->setCallBack([this]() { m_rock = nullptr; m_qte->setActive(false); ShowUI(true); });
	m_player->setActive(false);

	m_qte = CreateEntity<QTEEntity>(1, sf::Color::Transparent);
}

void LvL1Scene::InitCam()
{
	MyGameScene::InitCam();
	m_cam->zoom(6.8);
}

void LvL1Scene::lunchWaitingScreen()
{
	TransitionManager::getInstance().SetCallBack(nullptr);

	m_loading = true;
	m_player->SetPosition(0, 0);
	m_timer = 0;
}

void LvL1Scene::OnInitialize()
{

	MyGameScene::OnInitialize();

	//init sounds
	getAudioMap()->addSound("FootStep", {"res/Audio/Sounds/Step/foot.mp3"});
	getAudioMap()->setVolume("FootStep", 0.3);
	getAudioMap()->addSound("Dash", { "res/Audio/Sounds/Whoosh & Slash/Whoosh.wav" });
	getAudioMap()->addSound("AddManaSound", { "res/Audio/Sounds/Magic & Skill/Fx.wav" });
	getAudioMap()->setVolume("AddManaSound", 0.5);
	getAudioMap()->addSound("Ambiance", { "res/Audio/Sounds/Ambient/AmbianceSound.wav" });
	getAudioMap()->setVolume("Ambiance", 0.5);

	
	//init Paralax
	ParallaxManager::getInstance().init("res/Background/BackgroundCave");
	

	InitPlayer();
	ParseMap("res/maps/LvL1.tmx");
	SetUpUI();
	InitCam();


	m_rock->SetPosition(getPlayer()->GetPosition());

	SetGravity({ 0, 12 });

	TransitionManager::getInstance().SetFadeOut(true);

	ShowUI(false);
}

void LvL1Scene::OnUpdate()
{
	MyGameScene::OnUpdate();

	if (m_rock) {
		if (InputManager::getInstance().isJoystickButtonDown(JoystickButton::Sud) || InputManager::getInstance().isKeyDown(sf::Keyboard::A)) {
			m_rock->Damage(1);
			m_cam->zoom(0.9);
		}
	}

	m_cam->setPosition(m_player->GetPosition() + m_player->getCameraOffSet());
	m_cam->CorrectPosition(sf::Vector2f(m_map.width * m_map.tileWidth, m_map.height * m_map.tileHeight));

	if (m_qte && m_qte->isActive())
	{
		sf::View v = m_cam->getView();
		m_qte->SetPosition(v.getCenter().x, v.getCenter().y - 75);
	}
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
			GameManager::Get()->LaunchScene<LvL2Scene>();
	}
}

void LvL1Scene::OnWindowEvent(const sf::Event& event)
{

	MyGameScene::OnWindowEvent(event);
}
