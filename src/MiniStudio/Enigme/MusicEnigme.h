#pragma once
#include "Core/Entity.h"
#include <deque>
class Notes;

class MusicEnigme : public Entity
{
private: 

	bool m_visibility;
	float m_timer = 0;
	float m_inter = 1;
	float m_noteVel;

	int m_score;
	int m_goal;
	Entity* m_hitline;
	sf::Sprite* m_background;
	
	std::deque<int> m_notes;
	Notes* m_lastNote;

	std::function<void()> m_onWin;
	std::function<void()> m_onLose;

public:
	void init(bool _visibility, const std::deque<int>& _notes, float _inter, float _noteVelocity, int _scoreMinToWin, std::function<void()> _onWin, std::function<void()> _onLose);
	void OnInitialize() override;
	void OnFixedUpdate() override;

	void OnDestroy() override;

private:
};

