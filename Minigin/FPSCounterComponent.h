#pragma once
#include "BaseComponent.h"
#include <chrono>

namespace dae
{
    class FPSCounterComponent : public BaseComponent
    {
    public:
        FPSCounterComponent(GameObject* pOwner);
        virtual ~FPSCounterComponent() = default;

        void Update(float deltaTime) override;

        // Return FPS as a float for decimal precision
        float GetFPS() const { return m_FPS; }

    private:
        float m_FPS{ 0.0f }; // FPS as a float
        float m_ElapsedTime{ 0.0f }; // Time elapsed since last FPS update
        int m_FrameCount{ 0 }; // Number of frames since last FPS update

        std::chrono::high_resolution_clock::time_point m_LastTime; // Last time the FPS was calculated
    };
}
