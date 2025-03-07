#pragma once
#include <iostream>
class Command
{
public:
    virtual ~Command() = default;
    virtual void Execute() = 0;
};

