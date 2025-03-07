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
    void BindCommand(int button, std::unique_ptr<Command> command);
    int GetIndex() const;

    static int GetConnectedControllerCount();

private:
    int m_Index;
    SDL_GameController* m_Controller;
    std::unordered_map<int, std::unique_ptr<Command>> m_ButtonCommands;
};
