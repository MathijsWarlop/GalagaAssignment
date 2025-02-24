#pragma once
#include "BaseComponent.h"
#include <memory>
#include <string>
#include <SDL.h>

namespace dae
{
    class Font;
    class Texture2D;

    class TextRendererComponent final : public BaseComponent
    {
    public:
        TextRendererComponent(GameObject* pOwner, const std::string& text, float x, float y);
        ~TextRendererComponent() override = default;

        void Render() const override;
        void Update(float deltaTime) override;
        void SetText(const std::string& text);
        void SetPosition(float x, float y);

    private:
        void UpdateTexture(); // Internal helper function

        std::string m_Text;
        float m_X, m_Y;
        static std::shared_ptr<Font> m_Font;  // Static font shared by all instances
        std::shared_ptr<Texture2D> m_TextTexture;
        bool m_NeedsUpdate{ true };
    
    };
}
