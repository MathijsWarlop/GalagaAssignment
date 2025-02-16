#include "FPSCounterComponent.h"


dae::FPSCounterComponent::FPSCounterComponent(GameObject* pOwner)
    : BaseComponent(pOwner)
{
}

void dae::FPSCounterComponent::Update()
{
    using namespace std::chrono;

    // Get the current time
    auto currentTime = high_resolution_clock::now();

    // Compute delta time
    float deltaTime = duration<float>(currentTime - m_LastTime).count();
    m_LastTime = currentTime; // Update last time

    // Increment frame count
    ++m_FrameCount;
    m_ElapsedTime += deltaTime;

    // Update FPS every second
    if (m_ElapsedTime >= 1.0f)
    {
        m_FPS = m_FrameCount; // Set FPS value
        m_FrameCount = 0; // Reset frame count
        m_ElapsedTime = 0.0f; // Reset elapsed time
    }
}