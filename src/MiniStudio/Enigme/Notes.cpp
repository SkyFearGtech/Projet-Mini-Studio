#include "Enigme/Notes.h"
#include "Core/Debug.h"
#include "random"

#include "Input\InputManager.h"
#include "Assets\AssetsManager.h"
void Notes::init(bool _isVisible, int _lane, float _noteVelocity,Entity* _hitZone, sf::Vector2f _pos, std::function<void()> _onSuccces, std::function<void()> _onFail)
{
	SetVelocityY(_noteVelocity);
	m_lane = _lane;
	m_hitZone = _hitZone;
	m_onSucces = _onSuccces;
	m_onFail = _onFail;
	SetPosition(_pos.x + 70 + 66 * m_lane, _pos.y + 40);
	
	if (_isVisible) {
		switch (m_lane)
		{
		case(1):
			SetTexture(AssetsManager::getInstance().getTexture("YButton", "res/Ui/YButton.png"));
			break;
		case(2):
			SetTexture(AssetsManager::getInstance().getTexture("BButton", "res/Ui/BButton.png"));
			break;
		case(3):
			SetTexture(AssetsManager::getInstance().getTexture("AButton", "res/Ui/AButton.png"));
			break;
		case(4):
			SetTexture(AssetsManager::getInstance().getTexture("XButton", "res/Ui/XButton.png"));
			break;
		}
		GetAnimator().addClip("Idle", { 0 }, 0.1, false, mSprite.getTexture()->getSize().x, mSprite.getTexture()->getSize().y);
		GetAnimator().play("Idle");
		GetAnimator().applyScale(mSprite, GetSize());
	}
}
void Notes::OnInitialize()
{
	SetAffectedByGravity(false);
	SetAffectedByFriction(false);
	SetRigidBody(false);
}

void Notes::OnFixedUpdate()
{
	if (m_check == false && IsColliding(m_hitZone))
		m_check = true;

	if (m_check && IsColliding(m_hitZone)) {
		JoystickButton button;
		sf::Keyboard::Key key;
		switch (m_lane)
		{
		case(1):
			button = JoystickButton::Nord;
			key = sf::Keyboard::Y;
			break;
		case(2):
			button = JoystickButton::Est;
			key = sf::Keyboard::B;
			break;
		case(3):
			button = JoystickButton::Sud;
			key = sf::Keyboard::A;
			break;
		case(4):
			button = JoystickButton::Ouest;
			key = sf::Keyboard::X;
			break;
		}
		if (InputManager::getInstance().isKeyDown(key) || InputManager::getInstance().isJoystickButtonDown(button)) {
			if (m_onSucces)
				m_onSucces();
			Destroy();
		}
	}

	if (m_check && IsColliding(m_hitZone) == false) {
		if (m_onFail)
			m_onFail();
		Destroy();
	}
}
