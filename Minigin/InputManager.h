#pragma once

#include "Singleton.h"
#include "Command.h"
#include "Controller.h"
#include <unordered_map>
#include <memory>
#include <vector>
#include <iostream>

namespace dae
{
    class InputManager final : public Singleton<InputManager>
    {
    public:
        InputManager();
        ~InputManager();

        bool ProcessInput();

        // Combine button/key binding functions into one
        void BindCommand(int inputId, std::unique_ptr<Command> command, bool isButton);

    private:
        void HandleControllerEvent(int controllerIndex, bool connected);

        std::unordered_map<int, std::unique_ptr<Command>> m_ButtonCommands;
        std::unordered_map<int, std::unique_ptr<Command>> m_KeyCommands;

        std::vector<std::unique_ptr<Controller>> m_controllers;
    };
}
