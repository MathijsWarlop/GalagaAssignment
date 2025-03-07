#pragma once
#include "Singleton.h"
#include "Command.h"
#include <SDL.h>
#include <unordered_map>
#include <memory>
#include <iostream>

namespace dae
{
    class InputManager final : public Singleton<InputManager>
    {
    public:
        InputManager();
        ~InputManager();

        bool ProcessInput();
        void BindCommand(SDL_GameControllerButton button, std::unique_ptr<Command> command); //controller
        void BindCommand(SDL_Scancode key, std::unique_ptr<Command> command); //Keyboard
        void UnbindCommand(SDL_GameControllerButton button);

    private:
        void HandleControllerEvent(const SDL_Event& event);
        void ProcessControllerInput();

        std::unordered_map<SDL_GameControllerButton, std::unique_ptr<Command>> m_ButtonCommands; //Controller
        std::unordered_map<SDL_Scancode, std::unique_ptr<Command>> m_KeyCommands; //Keyboard

        SDL_GameController* m_pController = nullptr;
    };
}