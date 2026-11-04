#pragma once
#include "Core/Scene.h"
#include "Input/ActionMap.h"
class Player;
struct TmxObject;

class MyGameScene : public Scene {
protected:

	enum class Action {
		Pause,
	};





	sf::Shader m_invertShader;

	sf::Sprite m_loadingScreenSprite;
	sf::Sprite m_deathScreenSprite;

	//UI
	sf::Sprite m_healthBar;
	sf::Sprite m_heart;
	sf::Sprite m_mana;
	sf::Sprite m_WeaponSlot;
	sf::Sprite m_firstWeapon;
	std::vector<sf::Sprite> m_weapons;
	std::vector<sf::Vector2f> m_weaponPos;
	std::vector<sf::Vector2f> m_heartPos;
	std::vector<sf::Vector2f> m_manaPos;
	std::vector<sf::Vector2i> m_manaRect;



	MusicEnigme* enigme1;


	ActionMap<Action> m_actionMap;

	Player* m_player;

	bool m_loading = false;

	bool m_onZoom;
	float m_zoomFactor;
	float m_zoomSpeed;
	float m_zoomTimer;
	float m_timer;

	bool m_showUI = true;

public:


	void DrawUI(sf::RenderTarget& target) override;
	void OnDraw(sf::RenderTarget& target) override;
	MusicEnigme* getEnigme() { return enigme1; }
	void setEnigme(MusicEnigme* _enigme) { enigme1 = _enigme; }
	Player* getPlayer() const { return m_player; }

	void ShowUI(bool _state) { m_showUI = _state; }
	bool ShowUI() const { return m_showUI; }


protected:
	virtual void OnInitialize() override;
	virtual void OnUpdate() override;
	virtual void SetUpUI();
	virtual void InitCam();
	virtual void InitPlayer(bool _active = true);
	virtual void ParseMap(std::string _path);
	virtual void OnCustomSpawn(const TmxObject& obj) {}

	void lunchWaitingScreen();


	// H�rit� via Scene
	void OnWindowEvent(const sf::Event& event) override;

};