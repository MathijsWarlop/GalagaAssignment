#pragma once

#include "Command.h"
#include <SDL.h>
#include <unordered_map>
#include <memory>

class Controller
{
public:
    explicit Controller(int index);
    ~Controller();

    void ProcessInput();

    // Updated to support press, hold, and release
    void BindCommand(int button, std::unique_ptr<Command> pressCommand,
        std::unique_ptr<Command> holdCommand,
        std::unique_ptr<Command> releaseCommand);

    int GetIndex() const;
    static int GetConnectedControllerCount();

private:
    int m_Index;
    SDL_GameController* m_Controller;
    
    //std::unordered_map<int, bool> m_HoldCommandsInProgress; // track holdstates
    std::unordered_map<int, bool> m_PreviousButtonStates; // Track previous state
    std::unordered_map<int, std::unique_ptr<Command>> m_PressCommands;
    std::unordered_map<int, std::unique_ptr<Command>> m_HoldCommands;
    std::unordered_map<int, std::unique_ptr<Command>> m_ReleaseCommands;
};
