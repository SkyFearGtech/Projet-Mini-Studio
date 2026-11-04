#pragma once
#include "Core/Entity.h"

#include <functional>

class Notes : public Entity
{
private:
	int m_lane;
	bool m_check = false;
	Entity* m_hitZone;

	std::function<void()> m_onSucces;
	std::function<void()> m_onFail;

public:
	void init(bool _isVisible, int _lane, float _noteVelocity, Entity* _hitZone, sf::Vector2f _pos, std::function<void()> _onSuccces = nullptr, std::function<void()> _onFail = nullptr);
	void OnInitialize() override; 
	void OnFixedUpdate()override;
};

