#include "Enigme/MusicEnigme.h"
#include "Assets/AssetsManager.h"
#include "Input/InputManager.h"
#include "Player/Player.h"

#include "Enigme/Notes.h"

#include "Core/Debug.h"
#include "Scenes\MyGameScene.h"

#include <iostream>

void MusicEnigme::init(bool _visibility, const std::deque<int>& _notes,float _inter, float _noteVelocity, int _scoreMinToWin, std::function<void()> _onWin, std::function<void()> _onLose)
{
	m_visibility = _visibility;
	m_inter = _inter;
	m_notes = _notes;
	m_goal = _scoreMinToWin;
	m_onWin = _onWin;
	m_onLose = _onLose;
	m_noteVel = _noteVelocity;
}

void MusicEnigme::OnInitialize()
{

	Player* player = GetScene<MyGameScene>()->getPlayer();

	if (!player)
	{
		Destroy();
		return;
	}
	//setup du parchemin
	SetRigidBody(false);
	SetAffectedByFriction(false);
	SetAffectedByGravity(false);
	SetTexture(AssetsManager::getInstance().getTexture("MusicBG", "res/Ui/Background.png"));
	GetSprite().setScale(0.75f, 0.75f);
	
	LightSource m_bgLight;
	m_bgLight.color = sf::Color(222, 245, 130);
	m_bgLight.radius = 450.f;
	m_bgLight.isActive = true;
	m_bgLight.offset = sf::Vector2f(mSprite.getGlobalBounds().width / 2, mSprite.getGlobalBounds().height / 2);
	setLightSource(m_bgLight);

	SetPosition(player->GetPosition(4.5, -2.5), 0, 0);


	m_hitline = CreateEntity<Entity>({ 440,40}, sf::Color::Transparent);

	m_hitline->SetAffectedByGravity(false);
	m_hitline->SetRigidBody(false);
	m_hitline->SetPosition(GetPosition(0, 0.75), 0, 0);


}

void MusicEnigme::OnFixedUpdate()
{

	if (m_lastNote && m_lastNote->ToDestroy())
		m_lastNote = nullptr;

	m_timer += GameManager::Get()->GetDeltaTime();
	if (m_timer < m_inter) return;

	if (m_notes.empty()) {
		if (m_lastNote != nullptr) return;

		if (m_score >= m_goal)
			m_onWin();
		else
			m_onLose();
		Destroy();
		return;
	}

	int note = m_notes.front();
	m_notes.pop_front();

	if (note < 1 || note > 4) return; //error, on skip

	m_lastNote = CreateEntity<Notes>(sf::Vector2f(80, 80), sf::Color::Transparent);
	m_lastNote->init(m_visibility, note, m_noteVel,m_hitline, GetPosition(0, 0), [this] {m_score++;}, [this] {m_score--;});

	m_timer -= m_inter;
}



void MusicEnigme::OnDestroy()
{
	m_hitline->Destroy();
	GetScene<MyGameScene>()->setEnigme(nullptr);
}

