#pragma once
#include "FPSCounterComponent.h"
#include "TextRendererComponent.h"  // Ensure this is included
#include "GameObject.h"


namespace dae
{
    FPSCounterComponent::FPSCounterComponent(GameObject* pOwner)
        : BaseComponent(pOwner), m_LastTime(std::chrono::high_resolution_clock::now())
    {
    }

    void FPSCounterComponent::Update(float deltaTime)
    {
        m_FrameCount++;
        m_ElapsedTime += deltaTime;

       // std::cout << "Elapsed Time: " << m_ElapsedTime << " | Frame Count: " << m_FrameCount << std::endl;

        if (m_ElapsedTime >= 1.0f)
        {
            m_FPS = static_cast<float>(m_FrameCount) / m_ElapsedTime;
           // std::cout << "FPS: " << m_FPS << std::endl;

            // Get the TextRendererComponent to update the FPS on screen
            auto textComponent = m_pOwner->GetComponent<dae::TextRendererComponent>();
            if (textComponent)
            {
                textComponent->SetText("FPS: " + std::to_string(static_cast<int>(m_FPS)));
            }

            m_FrameCount = 0;
            m_ElapsedTime = 0.0f;
        }
    }
}
