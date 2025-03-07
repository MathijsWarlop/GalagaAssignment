#include "Controller.h"
#include <iostream>

Controller::Controller(int index)
    : m_Index(index), m_Controller(nullptr)
{
    if (SDL_IsGameController(index))
    {
        m_Controller = SDL_GameControllerOpen(index);
        if (!m_Controller)
        {
            std::cerr << "Failed to open controller " << index << ": " << SDL_GetError() << std::endl;
        }
        else
        {
            std::cout << "Controller " << index << " initialized successfully!" << std::endl;
        }
    }
}


Controller::~Controller()
{
    if (m_Controller)
    {
        SDL_GameControllerClose(m_Controller);
    }
}

void Controller::ProcessInput()
{
    if (!m_Controller) return;

    // Process controller buttons
    for (const auto& [button, command] : m_ButtonCommands)
    {
        if (command && SDL_GameControllerGetButton(m_Controller, static_cast<SDL_GameControllerButton>(button)))
        {
            command->Execute();
        }
    }
}


void Controller::BindCommand(int button, std::unique_ptr<Command> command)
{
    m_ButtonCommands[button] = std::move(command);
}

int Controller::GetIndex() const
{
    return m_Index;
}

int Controller::GetConnectedControllerCount()
{
    return SDL_NumJoysticks();
}
