#pragma once
#include "BaseComponent.h"
#include <memory>
#include <string>

namespace dae
{
    class TextObject;

    class TextRendererComponent : public BaseComponent
    {
    public:
        TextRendererComponent(GameObject* pOwner, const std::string& text, float x, float y);
        void Render() const override;
        void Update() override;
        void SetText(const std::string& text);

    private:
        std::shared_ptr<TextObject> m_pTextObject;
        std::string m_Text;  // Store text content
        float m_X{}, m_Y{};  // Store position
    };
}
