#pragma once
#include "GameObject.h"

namespace dae
{
    class FPSCounterComponent;
    class TextRendererComponent;

    class FPSCounter : public GameObject
    {
    public:
        FPSCounter();
        void Update() override;
        void Render() const override;

    private:
        FPSCounterComponent* m_pFPSCounter{};
        TextRendererComponent* m_pTextRenderer{};
    };
}