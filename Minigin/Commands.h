#pragma once
#include "Command.h"
#include <iostream>

class JumpCommand : public Command
{
public:
    void Execute() override
    {
        std::cout << "Player jumps!\n";
    }
};

class ShootCommand : public Command
{
public:
    void Execute() override
    {
        std::cout << "Player shoots!\n";
    }
};
