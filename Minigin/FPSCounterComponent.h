// FPSCounterComponent.h
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

        void Update() override;

        int GetFPS() const { return m_FPS; }

    private:
        int m_FPS{ 0 };
        float m_ElapsedTime{ 0.0f };
        int m_FrameCount{ 0 };
        std::chrono::high_resolution_clock::time_point m_LastTime;
    };
}
