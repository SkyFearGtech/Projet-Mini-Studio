#include "Core/GameManager.h"

#include "Scenes/LvL1Scene.h"

#include <crtdbg.h>


int main()
{
	GameManager* pInstance = GameManager::Get();

	pInstance->CreateWindow(1280, 720, "Gostelium", "res/Ui/LogoScreen/Logo.png", 60, sf::Color(128, 128, 128));

	pInstance->LaunchScene<LvL1Scene>();


	pInstance->SetUp();
	return 0;
}
