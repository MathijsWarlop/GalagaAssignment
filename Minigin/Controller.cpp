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

    SDL_GameControllerUpdate();  

    for (const auto& [button, _] : m_PressCommands)
    {
        bool isPressed = SDL_GameControllerGetButton(m_Controller, static_cast<SDL_GameControllerButton>(button));
        bool wasPressed = m_PreviousButtonStates[button];

        // Debugging: Output the state of the button
        //std::cout << "Button " << button << " is " << (isPressed ? "pressed" : "not pressed")
        //    << ", was " << (wasPressed ? "pressed" : "not pressed") << std::endl;

        if (isPressed && !wasPressed)  // Button just pressed
        {
            if (m_PressCommands[button]) {
                m_PressCommands[button]->Execute();
                //std::cout << "Press command executed!" << std::endl;
            }
        }
        else if (isPressed && wasPressed)  // Button held
        {
            if (m_HoldCommands[button]) {
                m_HoldCommands[button]->Execute();
               // std::cout << "Hold command executed!" << std::endl;
            }
        }
        else if (!isPressed && wasPressed)  // Button released
        {
            if (m_ReleaseCommands[button]) {
                m_ReleaseCommands[button]->Execute();
                //std::cout << "Release command executed!" << std::endl;
            }
        }

        // Update the previous state
        m_PreviousButtonStates[button] = isPressed;
    }
}

void Controller::BindCommand(int button, std::unique_ptr<Command> pressCommand,
    std::unique_ptr<Command> holdCommand,
    std::unique_ptr<Command> releaseCommand)
{
    if (pressCommand)   m_PressCommands[button] = std::move(pressCommand);
    if (holdCommand)    m_HoldCommands[button] = std::move(holdCommand);
    if (releaseCommand) m_ReleaseCommands[button] = std::move(releaseCommand);
}

void Controller::UnbindCommand(int button)
{
    m_PressCommands.erase(button);
    m_HoldCommands.erase(button);
    m_ReleaseCommands.erase(button);
}

int Controller::GetIndex() const
{
    return m_Index;
}

int Controller::GetConnectedControllerCount()
{
    return SDL_NumJoysticks();
}
