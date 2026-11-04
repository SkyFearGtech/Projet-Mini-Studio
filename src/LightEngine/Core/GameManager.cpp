#include "Core/GameManager.h"
#include "Graphics/ParallaxManager.h"
#include "Core/Entity.h"
#include "Core/Debug.h"
#include "UI/DialogueManager.h"
#include "Input/InputManager.h"
#include "Graphics/LightManager.h"
#include "Core/QuadTree.h"
#include "UI/TransitionManager.h"

#include <algorithm>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


#include "UI/UIElem.h"

GameManager::GameManager()
{
	mQuadTree = nullptr;
	mpWindow = nullptr;
	mDeltaTime = 0.0f;
	mpScene = nullptr;
	mpNextScene = nullptr;
	mAccumulatedDeltaTime = 0;
	mWindowWidth = -1;
	mWindowHeight = -1;
}

GameManager* GameManager::Get()
{
	static GameManager mInstance;

	return &mInstance;
}

GameManager::~GameManager()
{
	delete mpWindow;
	delete mpScene;

	for (Entity* entity : mEntities)
	{
		delete entity;
	}
}

void GameManager::CreateWindow(unsigned int width, unsigned int height, const char* title, const char* _iconPath, int fpsLimit, sf::Color clearColor)
{
	_ASSERT(mpWindow == nullptr);

	mpWindow = new sf::RenderWindow(sf::VideoMode(width, height), title);
	mpWindow->setFramerateLimit(fpsLimit);

	if (_iconPath != nullptr);
	{
		sf::Image icon;
		icon.loadFromFile(_iconPath);

		mpWindow->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	}

	mWindowWidth = width;
	mWindowHeight = height;

	mClearColor = clearColor;
}


void GameManager::SetUp()
{
	mpScene = mpNextScene;
	mpNextScene = nullptr;

	mpScene->SetGameManager(this);
	mpScene->OnInitialize();

	Run();
}

float timer = 0.0f;
int FPS = 0;

void GameManager::Run()
{
	if (mpWindow == nullptr)
	{
		std::cout << "Window not created, creating default window" << std::endl;
		CreateWindow(1280, 720, "Default window");
	}

	//#TODO : Load somewhere else
	bool fontLoaded = mFont.loadFromFile("res/Hack-Regular.ttf");
	_ASSERT(fontLoaded);

	_ASSERT(mpScene != nullptr);

	sf::Clock clock;
	InputManager& inputManager = InputManager::getInstance();

	DialogueManager& dialogueManager = DialogueManager::getInstance();
	LightManager& lightManager = LightManager::getInstance();
	TransitionManager& transitionManager = TransitionManager::getInstance();

	ParallaxManager& parallaxManager = ParallaxManager::getInstance();
	UIManager& uiManager = UIManager::getInstance();

	while (mpWindow->isOpen())
	{

		//We cap the dt at 0.05 so that if there's a big lag, it doesn't generate too many frames
		SetDeltaTime(std::min(clock.restart().asSeconds(), 0.05f));


		timer += mDeltaTime;
		if (timer > 1.0f) {
			FPS = (int)(1.0 / mDeltaTime);
			timer = 0.0f;
		}

		if (mShowDebug)
			Debug::DrawText(mpWindow->getView().getCenter().x + 900, mpWindow->getView().getCenter().y - 800, "FPS: " + std::to_string(FPS), sf::Color::White);




		inputManager.update();

		dialogueManager.update();

		transitionManager.update(GetDeltaTime(), mpWindow->getView().getCenter());

		parallaxManager.update();

		uiManager.update();

		Update();

		if (mpNextScene != nullptr)
		{
			//	delete all entity from the previous scene
			for (Entity* e : mEntities) delete e;
			for (Entity* e : mEntitiesToAdd) delete e;
			for (Entity* e : mEntitiesToDestroy) delete e;
			mEntities.clear();
			mEntitiesToAdd.clear();
			mEntitiesToDestroy.clear();

			lightManager.clearUiLights();
			lightManager.clearEntityLights();

			//	take the cam to use it in the next scene
			std::shared_ptr<Camera> cam = mpScene->getCam();

			//	change current scene
			delete mpScene;
			mpScene = mpNextScene;

			mpScene->setCam(cam);

			mpNextScene = nullptr;
			mpScene->SetGameManager(this);
			mpScene->OnInitialize();
		}

		Draw();


	}


}

void GameManager::setCamera(const Camera& cam)
{
	mpWindow->setView(cam.getView());
}

void GameManager::Update()
{


	mpScene->OnUpdate();

	mAccumulatedDeltaTime += mDeltaTime;
	while (mAccumulatedDeltaTime > kFixedDt)
	{
		// gravity
		for (Entity* entity : mEntities)
			entity->ApplyGravity();

		//shift
		FixedUpdate();

		if (mQuadTree == nullptr) {
			TmxMap* map = mpScene->GetMap();
			mQuadTree = new QuadTree(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(map->width * map->tileWidth, map->height * map->tileHeight)), 0);
		}
		mQuadTree->clear();
		for (Entity* e : mEntities)
			mQuadTree->insert(e, e->GetBounds());

		for (Entity* e : mEntities)
		{
			if (!e->IsRigidBody()) continue;
			auto candidates = mQuadTree->query(e->GetBounds());
			for (Entry& c : candidates)
			{
				if (c.entity == e || !c.entity->IsRigidBody()) continue;

				if (!e->IsColliding(c.entity)) continue;

				if (e->IsStatic() && c.entity->IsStatic()) continue;

				if (e->mIsPlatform == false && c.entity->mIsPlatform == false)
					e->Repulse(c.entity);

				else if (e->GetIsPlatform()) {
					float prevBottom = c.entity->GetPosition(0, 1).y - c.entity->GetVelocity().y;
					if (prevBottom <= e->GetPosition(0, 0).y)
						e->Repulse(c.entity);
				}
				else if (c.entity->GetIsPlatform()) {
					float prevBottom = e->GetPosition(0, 1).y - e->GetVelocity().y;
					if (prevBottom <= c.entity->GetPosition(0, 0).y)
						c.entity->Repulse(e);
				}

				e->OnCollision(c.entity);
			}
		}

		mAccumulatedDeltaTime -= kFixedDt;
	}

	LightManager::getInstance().update();

	// delete entity
	for (auto it = mEntitiesToDestroy.begin(); it != mEntitiesToDestroy.end(); ++it)
	{
		delete* it;
	}



	mEntitiesToDestroy.clear();

	// add entity
	for (auto it = mEntitiesToAdd.begin(); it != mEntitiesToAdd.end(); ++it)
	{
		mEntities.push_back(*it);
	}

	mEntitiesToAdd.clear();
}

void GameManager::FixedUpdate()
{
	if (!pause) {
		for (auto it = mEntities.begin(); it != mEntities.end(); )
		{
			Entity* entity = *it;
			if (entity->isActive() == false) {
				it++;
				continue;
			}
			entity->FixedUpdate();
			if (entity->ToDestroy() == false)
			{
				++it;
				continue;
			}
			mEntitiesToDestroy.push_back(entity);
			it = mEntities.erase(it);
		}
	}
}

void GameManager::Draw()
{
	mpWindow->clear(mClearColor);

	sf::View cameraView = mpWindow->getView();
	sf::View defaultView = mpWindow->getDefaultView();

	mpWindow->setView(defaultView);

	ParallaxManager::getInstance().drawBack(*mpWindow);

	mpWindow->setView(cameraView);

	mpScene->OnDraw(*mpWindow);


	for (Entity* entity : mEntities)
	{
		if (entity->isActive() == false)
			continue;
		if (entity->HasTexture())
			mpWindow->draw(entity->mSprite);
		else
			mpWindow->draw(entity->GetShape());

		if (mShowDebug && entity->isRect())
			Debug::DrawRectangle(entity->GetPosition(0, 0).x, entity->GetPosition(0, 0).y, entity->GetSize().x, entity->GetSize().y, sf::Color::Red);
	}

	mpScene->DrawUI(*mpWindow);

	LightManager::getInstance().render(*mpWindow);


	cameraView = mpWindow->getView();
	defaultView = mpWindow->getDefaultView();

	mpWindow->setView(defaultView);

	ParallaxManager::getInstance().drawFront(*mpWindow);

	DialogueManager::getInstance().Draw(*mpWindow);

	UIManager::getInstance().render(*mpWindow);

	mpWindow->setView(cameraView);
	
	Debug::Get()->Draw(mpWindow);

	mpWindow->display();
}
