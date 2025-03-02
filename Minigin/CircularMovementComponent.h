#include "BaseComponent.h"
#include <cmath>
#include <iostream>

namespace dae
{
    class GameObject; // Forward declaration

    class CircularMovementComponent : public BaseComponent
    {
    public:
        // Constructor
        CircularMovementComponent(GameObject* pOwner, float radius, float speed)
            : BaseComponent(pOwner), m_Radius(radius), m_Speed(speed), m_Angle(0.0f)
        {
            // Store the initial position of the owner as the center of rotation
            m_Center = GetOwner()->GetPosition();
            //std::cout << m_Center.x << " " << m_Center.y << std::endl;
        }

        // Override the Update method to move the GameObject in a circle
        void Update(float deltaTime) override
        {
            // Calculate the new angle
            m_Angle += m_Speed * deltaTime; // Update the angle based on speed and delta time

            // Normalize the angle to keep it within 0-2PI
            if (m_Angle > 2 * M_PI)
                m_Angle -= float(2 * M_PI);

            // Calculate the new position relative to the center
            float newX = m_Center.x + m_Radius * std::cos(m_Angle);
            float newY = m_Center.y + m_Radius * std::sin(m_Angle);

            // Set the new position of the GameObject
            GetOwner()->SetPosition(newX, newY);
        }

    private:
        float m_Radius; // Radius of the circle
        float m_Speed;  // Speed of movement (in radians per second)
        float m_Angle;  // Current angle in radians
        glm::vec3 m_Center; // Center of rotation (initial position of the owner)
    };
}