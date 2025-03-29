#pragma once
#include "BaseComponent.h"
#include "TextRendererComponent.h"
#include <chrono>

namespace dae
{
    class FPSCounterComponent : public BaseComponent
    {
    public:
        FPSCounterComponent(GameObject* pOwner);
        void Update(float deltaTime) override;

    private:
        int m_FrameCount = 0;
        float m_ElapsedTime = 0.0f;
        float m_FPS = 0.0f;
        std::chrono::high_resolution_clock::time_point m_LastTime;
    };
}
