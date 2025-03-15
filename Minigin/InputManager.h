#pragma once

#include "Singleton.h"
#include "Command.h"
#include "Controller.h"
#include <unordered_map>
#include <memory>
#include <vector>
#include <iostream>
#include <set>

namespace dae
{
    class InputManager final : public Singleton<InputManager>
    {
    public:
        InputManager();
        ~InputManager();

        bool ProcessInput();

        // Bind commands for press, hold, and release actions
        void BindCommand(int button, std::unique_ptr<Command> pressCommand,
            std::unique_ptr<Command> holdCommand,
            std::unique_ptr<Command> releaseCommand, bool isButton);

        void UnbindCommand(int button, bool isControllerButton);

    private:
        void HandleControllerEvent(int controllerIndex, bool connected);
        std::set<int> m_HeldKeys;
        std::unordered_map<int, std::unique_ptr<Command>> m_PressCommands;
        std::unordered_map<int, std::unique_ptr<Command>> m_HoldCommands;
        std::unordered_map<int, std::unique_ptr<Command>> m_ReleaseCommands;

        std::unordered_map<int, bool> m_KeyStates; // Track key/button states
        std::vector<std::unique_ptr<Controller>> m_controllers;
    };
}
