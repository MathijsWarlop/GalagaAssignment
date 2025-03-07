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

class MoveUpCommand : public Command
{
public:
    explicit MoveUpCommand(std::shared_ptr<dae::GameObject> ship) : m_Ship(ship) {}
    void Execute() override
    {
        auto pos = m_Ship->GetPosition();
        m_Ship->SetPosition(pos.x, pos.y - 5.0f);  // Move up by 5 units
    }
private:
    std::shared_ptr<dae::GameObject> m_Ship;
};

class MoveDownCommand : public Command
{
public:
    explicit MoveDownCommand(std::shared_ptr<dae::GameObject> ship) : m_Ship(ship) {}
    void Execute() override
    {
        auto pos = m_Ship->GetPosition();
        m_Ship->SetPosition(pos.x, pos.y + 5.0f);  // Move down by 5 units
    }
private:
    std::shared_ptr<dae::GameObject> m_Ship;
};

class MoveLeftCommand : public Command
{
public:
    explicit MoveLeftCommand(std::shared_ptr<dae::GameObject> ship) : m_Ship(ship) {}
    void Execute() override
    {
        auto pos = m_Ship->GetPosition();
        m_Ship->SetPosition(pos.x - 5.0f, pos.y);  // Move left by 5 units
    }
private:
    std::shared_ptr<dae::GameObject> m_Ship;
};

class MoveRightCommand : public Command
{
public:
    explicit MoveRightCommand(std::shared_ptr<dae::GameObject> ship) : m_Ship(ship) {}
    void Execute() override
    {
        auto pos = m_Ship->GetPosition();
        m_Ship->SetPosition(pos.x + 5.0f, pos.y);  // Move right by 5 units
    }
private:
    std::shared_ptr<dae::GameObject> m_Ship;
};
