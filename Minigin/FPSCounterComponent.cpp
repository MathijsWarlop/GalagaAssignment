#include "FPSCounterComponent.h"

//dae::FPSCounterComponent::FPSCounterComponent(GameObject& pOwner)
dae::FPSCounterComponent::FPSCounterComponent(GameObject* pOwner)
    : BaseComponent(pOwner), m_LastTime(std::chrono::high_resolution_clock::now())
{
}

void dae::FPSCounterComponent::Update(float deltaTime)
{
    using namespace std::chrono;

    // Get the current time
    auto currentTime = high_resolution_clock::now();


    m_LastTime = currentTime; // Update last time

    // Increment frame count
    ++m_FrameCount;
    m_ElapsedTime += deltaTime;

    // Update FPS every second
    if (m_ElapsedTime >= 1.0f)
    {
        // Calculate FPS as frames per second (frames / elapsed time)
        m_FPS = static_cast<float>(m_FrameCount) / m_ElapsedTime;

        // Reset counters
        m_FrameCount = 0;
        m_ElapsedTime = 0.0f;
    }
}