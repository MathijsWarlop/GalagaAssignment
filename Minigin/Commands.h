#pragma once
#include <iostream>
#include "command.h"
#include "GameObject.h"
#include "HealthComponent.h" // Include HealthComponent header
#include "ScoreComponent.h"   // Include ScoreComponent header


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
class TakeDamageCommand : public Command {
public:
    TakeDamageCommand(const std::shared_ptr<dae::GameObject>& ship, int damage)
        : m_Ship(ship), m_Damage(damage) {
        if (!m_Ship) {
            std::cerr << "Error: Invalid GameObject passed to TakeDamageCommand!\n";
        }
    }

    void Execute() override {
        if (!m_Ship) {
            std::cerr << "Error: GameObject is null in TakeDamageCommand!\n";
            return;
        }
        auto health = m_Ship->GetComponent<dae::HealthComponent>();
        if (!health) {
            std::cerr << "Error: HealthComponent not found on GameObject: " << m_Ship.get() << "\n";
            return;
        }
        health->TakeDamage(m_Damage);
    }

private:
    std::shared_ptr<dae::GameObject> m_Ship;
    int m_Damage;
};


class GainPointsCommand : public Command {
public:
    GainPointsCommand(std::shared_ptr<dae::GameObject> ship, int points)
        : m_Ship(std::move(ship)), m_Points(points) {
    }

    void Execute() override {
        //std::cout << "score command executed\n";
        auto score = m_Ship->GetComponent<dae::ScoreComponent>();
        if (score) {
            score->AddPoints(m_Points);
        }
    }

private:
    std::shared_ptr<dae::GameObject> m_Ship;
    int m_Points;
};




