#include "InputManager.h"
#include "backends/imgui_impl_sdl2.h"
#include <iostream>
#include "Controller.h"

namespace dae
{
    InputManager::InputManager()
    {
        int numControllers = SDL_NumJoysticks();
        std::cout << "Number of connected controllers: " << numControllers << std::endl;

        for (int i = 0; i < numControllers; ++i)
        {
            if (SDL_IsGameController(i)) {
                m_controllers.emplace_back(std::make_unique<Controller>(i));
                std::cout << "Controller " << i << " connected!" << std::endl;
            }
            else {
                std::cout << "Controller " << i << " is not a valid game controller." << std::endl;
            }
        }
    }

    InputManager::~InputManager() {}

    bool InputManager::ProcessInput()
    {
        SDL_Event e;

        // Poll events for SDL_QUIT and keyboard events
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                std::cout << "Window closed, exiting.\n";
                return false;
            }

            if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
            {
                int key = e.key.keysym.scancode;
                bool isPressed = (e.type == SDL_KEYDOWN);

                if (isPressed)
                {
                    if (!m_KeyStates[key]) // Only add the key if it's not already held down
                    {
                        m_HeldKeys.insert(key); 
                        //std::cout << "Key " << key << " is pressed." << std::endl;
                    }
                    if (m_PressCommands[key]) {
                        m_PressCommands[key]->Execute();
                    }
                }
                else
                {
                    if (m_KeyStates[key]) // Only remove the key if it was previously held down
                    {
                        m_HeldKeys.erase(key); 
                        //std::cout << "Key " << key << " is released." << std::endl;
                    }
                    if (m_ReleaseCommands[key]) {
                        m_ReleaseCommands[key]->Execute();
                    }
                }
                m_KeyStates[key] = isPressed;
            }
        }

        // HOLD IMPUTS CONTROLLERS
        for (auto& controller : m_controllers)
        {
            controller->ProcessInput();
        }

        // HOLD IMPUTS KEYBOARD
        for (const auto& key : m_HeldKeys)
        {
            if (m_HoldCommands[key]) {
                m_HoldCommands[key]->Execute();
            }
        }
        return true;
    }

    void InputManager::BindCommand(int button, std::unique_ptr<Command> pressCommand,
        std::unique_ptr<Command> holdCommand,
        std::unique_ptr<Command> releaseCommand, bool isControllerButton)
    {
        if (isControllerButton)
        {
            // GET A WARNING WHEN LOOPING OVER EVERY CONTROLLER, CANT MOVE TO MULTIPLE CUZ ITS AN UNIQUE POINTER
            if (!m_controllers.empty() && m_controllers[0] != nullptr)
            {
                m_controllers[0]->BindCommand(button,
                    std::move(pressCommand),
                    std::move(holdCommand),
                    std::move(releaseCommand));
            }
            
        }
        else
        {
            if (pressCommand)
                m_PressCommands[button] = std::move(pressCommand);
            if (holdCommand)
                m_HoldCommands[button] = std::move(holdCommand);
            if (releaseCommand)
                m_ReleaseCommands[button] = std::move(releaseCommand);
        }
    }

    void InputManager::UnbindCommand(int button, bool isControllerButton)
    {
        if (isControllerButton)
        {
            if (!m_controllers.empty() && m_controllers[0] != nullptr)
            {
                m_controllers[0]->UnbindCommand(button);
            }
        }
        else
        {
            m_PressCommands.erase(button);
            m_HoldCommands.erase(button);
            m_ReleaseCommands.erase(button);
        }
    }

    void InputManager::HandleControllerEvent(int controllerIndex, bool connected)
    {
        if (connected)
        {
            m_controllers.emplace_back(std::make_unique<Controller>(controllerIndex));
            std::cout << "Controller " << controllerIndex << " connected!\n";
        }
        else
        {
            auto it = std::remove_if(m_controllers.begin(), m_controllers.end(),
                [&](const std::unique_ptr<Controller>& controller)
                {
                    return controller->GetIndex() == controllerIndex;
                });
            if (it != m_controllers.end())
            {
                m_controllers.erase(it);
                std::cout << "Controller " << controllerIndex << " disconnected!\n";
            }
        }
    }
}
