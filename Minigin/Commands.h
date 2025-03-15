#pragma once
#include <iostream>
#include "command.h"

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

class MoveCommand : public Command
{
public:
    MoveCommand(std::shared_ptr<dae::GameObject> ship, glm::vec2 direction, float speed)
        : m_Ship(std::move(ship)), m_Direction(direction), m_Speed(speed) {
    }

    void Execute() override
    {
        auto pos = m_Ship->GetPosition();
        m_Ship->SetPosition(pos.x + m_Direction.x * m_Speed, pos.y + m_Direction.y * m_Speed);
    }

private:
    std::shared_ptr<dae::GameObject> m_Ship;
    glm::vec2 m_Direction;
    float m_Speed;
};


//CHANGE THIS IN A SINGLE COMMAND USING A VECTOR FOR THE DIRECTION
    //class MoveUpCommand : public Command
    //{
    //public:
    //    explicit MoveUpCommand(std::shared_ptr<dae::GameObject> ship) : m_Ship(ship) {}
    //    void Execute() override
    //    {
    //        auto pos = m_Ship->GetPosition();
    //        m_Ship->SetPosition(pos.x, pos.y - 5.0f);  // Move up by 5 units
    //    }
    //private:
    //    std::shared_ptr<dae::GameObject> m_Ship;
    //};
    //
    //class MoveDownCommand : public Command
    //{
    //public:
    //    explicit MoveDownCommand(std::shared_ptr<dae::GameObject> ship) : m_Ship(ship) {}
    //    void Execute() override
    //    {
    //        auto pos = m_Ship->GetPosition();
    //        m_Ship->SetPosition(pos.x, pos.y + 5.0f);  // Move down by 5 units
    //    }
    //private:
    //    std::shared_ptr<dae::GameObject> m_Ship;
    //};
    //
    //class MoveLeftCommand : public Command
    //{
    //public:
    //    explicit MoveLeftCommand(std::shared_ptr<dae::GameObject> ship) : m_Ship(ship) {}
    //    void Execute() override
    //    {
    //        auto pos = m_Ship->GetPosition();
    //        m_Ship->SetPosition(pos.x - 5.0f, pos.y);  // Move left by 5 units
    //    }
    //private:
    //    std::shared_ptr<dae::GameObject> m_Ship;
    //};
    //
    //class MoveRightCommand : public Command
    //{
    //public:
    //    explicit MoveRightCommand(std::shared_ptr<dae::GameObject> ship) : m_Ship(ship) {}
    //    void Execute() override
    //    {
    //        auto pos = m_Ship->GetPosition();
    //        m_Ship->SetPosition(pos.x + 5.0f, pos.y);  // Move right by 5 units
    //    }
    //private:
    //    std::shared_ptr<dae::GameObject> m_Ship;
    //};

