#include "InputManager.h"
#include "backends/imgui_impl_sdl2.h"
#include <iostream>
#include "Controller.h"

namespace dae
{
    InputManager::InputManager()
    {
        // Check if controllers are connected
        int numControllers = SDL_NumJoysticks();
        std::cout << "Number of connected controllers: " << numControllers << std::endl;

        // Initialize controllers
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
        while (SDL_PollEvent(&e))
        {
            // Handle SDL_QUIT event (like clicking the close button)
            if (e.type == SDL_QUIT)
            {
                std::cout << "Window closed, exiting.\n";
                return false; // Stop the game loop and exit
            }

            // Process mouse button down and mouse motion events
            if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEMOTION)
            {
                if (e.type == SDL_MOUSEBUTTONDOWN)
                {
                    //std::cout << "Mouse button pressed at (" << e.button.x << ", " << e.button.y << ")\n";
                }
                if (e.type == SDL_MOUSEMOTION)
                {
                    //std::cout << "Mouse moved to (" << e.motion.x << ", " << e.motion.y << ")\n";
                }
            }

            // Process keyboard events
            if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
            {
                const SDL_Scancode scancode = e.key.keysym.scancode;
                if (SDL_GetKeyboardState(nullptr)[scancode])
                {
                    //std::cout << "Key pressed: " << SDL_GetKeyName(e.key.keysym.sym) << std::endl;
                }
            }

            // Process controller input
            for (auto& controller : m_controllers)
            {
                controller->ProcessInput();
            }

            // Check for keyboard command bindings
            for (const auto& [key, command] : m_KeyCommands)
            {
                if (command && SDL_GetKeyboardState(nullptr)[key])
                {
                    //std::cout << "Executing command for key " << key << std::endl;
                    command->Execute();
                }
            }
        }

        return true; // Continue the game loop if no quit event
    }





    void InputManager::BindCommand(int button, std::unique_ptr<Command> command, bool isButton)
    {
        if (isButton) // Controller button
        {
            m_ButtonCommands[button] = std::move(command);
            // Bind the command to all controllers
            for (auto& controller : m_controllers)
            {
                controller->BindCommand(button, std::move(m_ButtonCommands[button]));
            }
        }
        else // Keyboard key
        {
            m_KeyCommands[button] = std::move(command);
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
