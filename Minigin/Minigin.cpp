#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
//
#include "GameObject.h"
#include <chrono>
#include <thread>
#include "Scene.h"
#include "FPSCounterComponent.h"
#include "TextRendererComponent.h"
#include "CircularMovementComponent.h"
#include "HealthComponent.h"
#include "HealthObserver.h"
#include "Command.h"
#include "Commands.h"
//#include "GuiComponent.h"

SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
		std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
		return;  // Or handle the error appropriately
	}
}

dae::Minigin::Minigin(const std::string &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);
}

dae::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	load();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto scene = sceneManager.GetActiveScene(); // Get active scene

	//FPS COUNTER
	auto fpsCounterObject = std::make_shared<GameObject>();
	fpsCounterObject->AddComponent<FPSCounterComponent>();
	fpsCounterObject->AddComponent<TextRendererComponent>("FPS: 0", 2.f, 2.f);
	scene->Add(fpsCounterObject);

	//parent-child test (circling logos)
	//auto circleParent = std::make_shared<dae::GameObject>();
	//circleParent->SetTexture("logo.tga");
	//circleParent->SetPosition(216, 180);
	//circleParent->AddComponent<CircularMovementComponent>(60.f, 5.f);
	//auto circleChild = std::make_shared<dae::GameObject>();
	//circleChild->SetTexture("logo.tga");
	//circleChild->SetPosition(0, 0);
	//circleChild->AddComponent<CircularMovementComponent>(50.f, 9.f);
	//circleChild->SetParent(circleParent.get());
	//
	//scene->Add(circleParent);
	//scene->Add(circleChild);

	// Gui Graphs (week3)
	//auto GuiGraph = std::make_shared<dae::GameObject>();
	//GuiGraph->AddComponent<GuiComponent>();
	//scene->Add(GuiGraph);

	//SHIP 1 (Controller)
	// Create the ship
	auto ship1 = std::make_shared<dae::GameObject>();
	ship1->SetTexture("ship.png");
	ship1->SetPosition(200, 180);

	auto health =  ship1->AddComponent<dae::HealthComponent>(100); // 100 HP


	std::shared_ptr<HealthObserver> healthObserver = std::make_shared<HealthObserver>();
	ship1->RegisterObserver(healthObserver);
	scene->Add(ship1);
	health->TakeDamage(30);


	//SHIP 2 (Keyboard)
	auto ship2 = std::make_shared<dae::GameObject>();
	ship2->SetTexture("ship.png");
	ship2->SetPosition(400, 180); 
	ship2->AddComponent<dae::HealthComponent>(100); // 100 HP
	auto healthObserver2 = std::make_shared<HealthObserver>();
	ship2->RegisterObserver(healthObserver2);
	scene->Add(ship2);

	auto& input = InputManager::GetInstance();

	{ // IMPUTS -> First is PRESSED, second is HELD, thid one is RELEASED
		// Bind commands to controller buttons for ship1 (controller)
		constexpr float moveSpeed = 5.0f; // Adjust speed as needed

		// Controller Input for ship1
		input.BindCommand(SDL_CONTROLLER_BUTTON_DPAD_UP,
			std::make_unique<MoveCommand>(ship1, glm::vec2{ 0, -1 }, moveSpeed),
			std::make_unique<MoveCommand>(ship1, glm::vec2{ 0, -1 }, moveSpeed),
			nullptr, true);

		input.BindCommand(SDL_CONTROLLER_BUTTON_DPAD_DOWN,
			std::make_unique<MoveCommand>(ship1, glm::vec2{ 0, 1 }, moveSpeed),
			std::make_unique<MoveCommand>(ship1, glm::vec2{ 0, 1 }, moveSpeed),
			nullptr, true);

		input.BindCommand(SDL_CONTROLLER_BUTTON_DPAD_LEFT,
			std::make_unique<MoveCommand>(ship1, glm::vec2{ -1, 0 }, moveSpeed),
			std::make_unique<MoveCommand>(ship1, glm::vec2{ -1, 0 }, moveSpeed),
			nullptr, true);

		input.BindCommand(SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
			std::make_unique<MoveCommand>(ship1, glm::vec2{ 1, 0 }, moveSpeed),
			std::make_unique<MoveCommand>(ship1, glm::vec2{ 1, 0 }, moveSpeed),
			nullptr, true);

		// Keyboard Input for ship2
		input.BindCommand(SDL_SCANCODE_W,
			std::make_unique<MoveCommand>(ship2, glm::vec2{ 0, -1 }, moveSpeed),
			std::make_unique<MoveCommand>(ship2, glm::vec2{ 0, -1 }, moveSpeed),
			nullptr, false);

		input.BindCommand(SDL_SCANCODE_S,
			std::make_unique<MoveCommand>(ship2, glm::vec2{ 0, 1 }, moveSpeed),
			std::make_unique<MoveCommand>(ship2, glm::vec2{ 0, 1 }, moveSpeed),
			nullptr, false);

		input.BindCommand(SDL_SCANCODE_A,
			std::make_unique<MoveCommand>(ship2, glm::vec2{ -1, 0 }, moveSpeed),
			std::make_unique<MoveCommand>(ship2, glm::vec2{ -1, 0 }, moveSpeed),
			nullptr, false);

		input.BindCommand(SDL_SCANCODE_D,
			std::make_unique<MoveCommand>(ship2, glm::vec2{ 1, 0 }, moveSpeed),
			std::make_unique<MoveCommand>(ship2, glm::vec2{ 1, 0 }, moveSpeed),
			nullptr, false);

		// Controller Input for Testing (X = Take Damage, B = Gain Points)
		input.BindCommand(SDL_CONTROLLER_BUTTON_X,
			std::make_unique<TakeDamageCommand>(ship1, 10), // Ship takes 10 damage
			nullptr, nullptr, true);
		//input.BindCommand(SDL_CONTROLLER_BUTTON_B,
		//	std::make_unique<GainPointsCommand>(ship1, 100), // Ship gains 100 points
		//	nullptr, nullptr, true);

		// Keyboard Input for Testing (K = Take Damage, L = Gain Points)
		input.BindCommand(SDL_SCANCODE_K,
			std::make_unique<TakeDamageCommand>(ship2, 10), // Ship takes 10 damage
			nullptr, nullptr, false);
		//input.BindCommand(SDL_SCANCODE_L,
		//	std::make_unique<GainPointsCommand>(ship2, 100), // Ship gains 100 points
		//	nullptr, nullptr, false);

	}


	// new update loop
	const float fixed_time_step = 1.0f / 60.0f; // Example: 60 updates per second
	const int ms_per_frame = 1000 / 60; // ~16.67 milliseconds per frame for 60 FPS
	bool do_continue = true;
	auto last_time = std::chrono::high_resolution_clock::now();
	float lag = 0.0f;
	while (do_continue)
	{
		const auto current_time = std::chrono::high_resolution_clock::now();
		const float delta_time = std::chrono::duration<float>(current_time - last_time).count();
		last_time = current_time;
		lag += delta_time;
		do_continue = input.ProcessInput();
		while (lag >= fixed_time_step)
		{
			sceneManager.FixedUpdate(fixed_time_step); //in sceneManager
			lag -= fixed_time_step;
		}
		//fps update
		auto textRenderer = fpsCounterObject->GetComponent<TextRendererComponent>();
		auto fpsComponent = fpsCounterObject->GetComponent<FPSCounterComponent>();
		float fps = fpsComponent->GetFPS();
		textRenderer->SetText("FPS: " + std::to_string(fps));
	

		sceneManager.Update(delta_time);
		renderer.Render();
		const auto sleep_time = current_time + std::chrono::milliseconds(ms_per_frame) - std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(sleep_time);
	}
}
