//#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "Scene.h"
#include "GameObject.h"
//gameObjects
#include "FPSCounter.h"
//#include <steam_api.h>
#include <SDL_gamecontroller.h>
#include <SDL.h>
#include "InputManager.h" 
#include "GameObject.h"
#include "FPSCounterComponent.h"
#include "TextRendererComponent.h"
#include "Commands.h"
#include "Healthobserver.h"
#include "ScoreObserver.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "AchievementObserver.h"
using namespace dae;


void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();
	go->SetTexture("space.png");
	scene.Add(go);

	// FPS COUNTER
	auto fpsCounterObject = std::make_shared<GameObject>();
	fpsCounterObject->AddComponent<FPSCounterComponent>();
	fpsCounterObject->AddComponent<dae::TextRendererComponent>("FPS: 0", 2.f, 2.f);
	scene.Add(fpsCounterObject);

	// UI ELEMENTS
	auto ShipController1UIHealth = std::make_shared<GameObject>();
	ShipController1UIHealth->AddComponent<TextRendererComponent>("100", 62.f, 40.f);
	scene.Add(ShipController1UIHealth);

	auto ShipController1score = std::make_shared<GameObject>();
	ShipController1score->AddComponent<TextRendererComponent>("0", 62.f, 60.f);
	scene.Add(ShipController1score);

	auto ShipController2UIHealth = std::make_shared<GameObject>();
	ShipController2UIHealth->AddComponent<TextRendererComponent>("100", 560.f, 40.f);
	scene.Add(ShipController2UIHealth);

	auto ShipController2score = std::make_shared<GameObject>();
	ShipController2score->AddComponent<TextRendererComponent>("0", 560.f, 60.f);
	scene.Add(ShipController2score);

	// SHIP 1
	auto ship1 = std::make_shared<dae::GameObject>();
	ship1->SetTexture("ship.png");
	ship1->SetPosition(200, 180);
	ship1->AddComponent<dae::HealthComponent>(100);
	ship1->AddComponent<dae::ScoreComponent>();
	ship1->RegisterObserver(std::make_shared<HealthObserver>(ShipController1UIHealth));
	ship1->RegisterObserver(std::make_shared<ScoreObserver>(ShipController1score));
	scene.Add(ship1);

	// SHIP 2
	auto ship2 = std::make_shared<dae::GameObject>();
	ship2->SetTexture("ship.png");
	ship2->SetPosition(400, 180);
	ship2->AddComponent<dae::HealthComponent>(100);
	ship2->AddComponent<dae::ScoreComponent>();
	ship2->RegisterObserver(std::make_shared<HealthObserver>(ShipController2UIHealth));
	ship2->RegisterObserver(std::make_shared<ScoreObserver>(ShipController2score));
	scene.Add(ship2);

	// ACHIEVEMENTS
	auto achievementObserver = std::make_shared<AchievementObserver>();
	ship1->RegisterObserver(achievementObserver);
	ship2->RegisterObserver(achievementObserver);

	// INPUT BINDINGS
	auto& input = dae::InputManager::GetInstance();
	constexpr float moveSpeed = 5.0f;

	// Controller Input for Ship 1
	input.BindCommand(SDL_CONTROLLER_BUTTON_DPAD_UP,
		std::make_unique<MoveCommand>(ship1, glm::vec2{ 0, -1 }, moveSpeed),
		std::make_unique<MoveCommand>(ship1, glm::vec2{ 0, -1 }, moveSpeed), nullptr, true);
	input.BindCommand(SDL_CONTROLLER_BUTTON_DPAD_DOWN,
		std::make_unique<MoveCommand>(ship1, glm::vec2{ 0, 1 }, moveSpeed),
		std::make_unique<MoveCommand>(ship1, glm::vec2{ 0, 1 }, moveSpeed), nullptr, true);
	input.BindCommand(SDL_CONTROLLER_BUTTON_DPAD_LEFT,
		std::make_unique<MoveCommand>(ship1, glm::vec2{ -1, 0 }, moveSpeed),
		std::make_unique<MoveCommand>(ship1, glm::vec2{ -1, 0 }, moveSpeed), nullptr, true);
	input.BindCommand(SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
		std::make_unique<MoveCommand>(ship1, glm::vec2{ 1, 0 }, moveSpeed),
		std::make_unique<MoveCommand>(ship1, glm::vec2{ 1, 0 }, moveSpeed), nullptr, true);

	// Keyboard Input for Ship 2
	input.BindCommand(SDL_SCANCODE_W,
		std::make_unique<MoveCommand>(ship2, glm::vec2{ 0, -1 }, moveSpeed),
		std::make_unique<MoveCommand>(ship2, glm::vec2{ 0, -1 }, moveSpeed), nullptr, false);
	input.BindCommand(SDL_SCANCODE_S,
		std::make_unique<MoveCommand>(ship2, glm::vec2{ 0, 1 }, moveSpeed),
		std::make_unique<MoveCommand>(ship2, glm::vec2{ 0, 1 }, moveSpeed), nullptr, false);
	input.BindCommand(SDL_SCANCODE_A,
		std::make_unique<MoveCommand>(ship2, glm::vec2{ -1, 0 }, moveSpeed),
		std::make_unique<MoveCommand>(ship2, glm::vec2{ -1, 0 }, moveSpeed), nullptr, false);
	input.BindCommand(SDL_SCANCODE_D,
		std::make_unique<MoveCommand>(ship2, glm::vec2{ 1, 0 }, moveSpeed),
		std::make_unique<MoveCommand>(ship2, glm::vec2{ 1, 0 }, moveSpeed), nullptr, false);

	// Debug Inputs (Damage & Score)
	input.BindCommand(SDL_SCANCODE_K, std::make_unique<TakeDamageCommand>(ship2, 10), nullptr, nullptr, false);
	input.BindCommand(SDL_SCANCODE_L, std::make_unique<GainPointsCommand>(ship2, 100), nullptr, nullptr, false);

}

int main(int, char* []) {
	//if (!SteamAPI_Init())
	//{
	//	std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)." << std::endl;
	//	return 1;
	//}
	//else
	//	std::cout << "Successfully initialized steam." << std::endl;
	dae::Minigin engine("../Data/");
	engine.Run(load);

	//SteamAPI_Shutdown();
	return 0;
}