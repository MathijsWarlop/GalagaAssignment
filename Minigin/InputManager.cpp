#include "InputManager.h"
#include "backends/imgui_impl_sdl2.h"

dae::InputManager::InputManager()
{
    // Initialize SDL with controller support
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
        return;
    }

    // Check for already connected controllers
    for (int i = 0; i < SDL_NumJoysticks(); ++i)
    {
        if (SDL_IsGameController(i))
        {
            m_pController = SDL_GameControllerOpen(i);
            if (m_pController)
            {
                std::cout << "Controller connected!\n";
                break;
            }
        }
    }

 
}

dae::InputManager::~InputManager()
{
    if (m_pController)
    {
        SDL_GameControllerClose(m_pController);
    }
    SDL_Quit();
}

bool dae::InputManager::ProcessInput()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            return false;

        case SDL_CONTROLLERDEVICEADDED:
        case SDL_CONTROLLERDEVICEREMOVED:
            HandleControllerEvent(e);
            break;

        case SDL_KEYDOWN:
            if (m_KeyCommands.contains(e.key.keysym.scancode))
            {
                m_KeyCommands[e.key.keysym.scancode]->Execute();
            }
            break;
        }

        // Process ImGui events
        ImGui_ImplSDL2_ProcessEvent(&e);
    }

    // Process controller input
    ProcessControllerInput();

    return true;
}

void dae::InputManager::BindCommand(SDL_GameControllerButton button, std::unique_ptr<Command> command) //controller
{
    m_ButtonCommands[button] = std::move(command);
}

void dae::InputManager::BindCommand(SDL_Scancode key, std::unique_ptr<Command> command) //keyboard
{
    m_KeyCommands[key] = std::move(command);
}

void dae::InputManager::UnbindCommand(SDL_GameControllerButton button)
{
    m_ButtonCommands.erase(button); 
}


void dae::InputManager::HandleControllerEvent(const SDL_Event& event)
{
    if (event.type == SDL_CONTROLLERDEVICEADDED)
    {
        // A new controller has been connected
        if (!m_pController)
        {
            int deviceIndex = event.cdevice.which;
            m_pController = SDL_GameControllerOpen(deviceIndex);
            if (m_pController)
            {
                std::cout << "Controller connected!\n";
            }
            else
            {
                std::cerr << "Failed to open controller: " << SDL_GetError() << "\n";
            }
        }
    }
    else if (event.type == SDL_CONTROLLERDEVICEREMOVED)
    {
        // A controller has been disconnected
        if (m_pController && SDL_GameControllerGetAttached(m_pController) == SDL_FALSE)
        {
            SDL_GameControllerClose(m_pController);
            m_pController = nullptr;
            std::cout << "Controller disconnected!\n";
        }
    }
 
}

void dae::InputManager::ProcessControllerInput()
{
    if (m_pController)
    {
        for (const auto& [button, command] : m_ButtonCommands)
        {
            if (SDL_GameControllerGetButton(m_pController, button))
            {
                command->Execute();
            }
        }
    }
}