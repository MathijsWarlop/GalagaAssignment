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
            // Store the initial offset relative to the parent (if any)
            if (GetOwner()->GetParent())
            {
                m_Offset = GetOwner()->GetPosition() - GetOwner()->GetParent()->GetPosition();
            }
            else
            {
                m_Offset = GetOwner()->GetPosition();
            }
        }

        // Override the Update method to move the GameObject in a circle
        void Update() override
        {
            // Calculate the new angle
            m_Angle += m_Speed * 0.016f; // 0.016f is roughly the time for one frame at 60 FPS
            if (m_Angle > 2 * M_PI) // Normalize the angle to keep it within 0-2PI
                m_Angle -= float(2 * M_PI);

            // Calculate the new position relative to the parent (if any)
            float newX = m_Offset.x + m_Radius * std::cos(m_Angle);
            float newY = m_Offset.y + m_Radius * std::sin(m_Angle);

            // Set the new position of the GameObject
            GetOwner()->SetPosition(newX, newY);
        }

    private:
        float m_Radius; // Radius of the circle
        float m_Speed;  // Speed of movement (in radians per second)
        float m_Angle;   // Current angle in radians
        glm::vec3 m_Offset; // Initial offset relative to the parent (if any)
    };
}