#include "TextRendererComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Font.h"
#include "Texture2D.h"
#include <SDL_ttf.h>
#include <iostream>

std::shared_ptr<dae::Font> dae::TextRendererComponent::m_Font = nullptr;

dae::TextRendererComponent::TextRendererComponent(GameObject* pOwner, const std::string& text, float x, float y)
    : BaseComponent(pOwner), m_Text(text), m_X(x), m_Y(y)
{
    if (!m_Font)  // Load font only once
    {
        m_Font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
    }
    UpdateTexture();
   
}

void dae::TextRendererComponent::Render() const
{
    if (m_TextTexture)
    {
        Renderer::GetInstance().RenderTexture(*m_TextTexture, m_X, m_Y);
    }
}

void dae::TextRendererComponent::Update(float)
{
    if (m_NeedsUpdate)
    {
        UpdateTexture();
    }
}

void dae::TextRendererComponent::SetText(const std::string& text)
{
    if (m_Text != text)
    {
        m_Text = text;
        m_NeedsUpdate = true;
    }
}

void dae::TextRendererComponent::SetPosition(float x, float y)
{
    m_X = x;
    m_Y = y;
}

void dae::TextRendererComponent::UpdateTexture()
{
    if (!m_Font)
    {
        std::cerr << "TextRendererComponent::UpdateTexture - Font is null!\n";
        return;
    }

    SDL_Color color = { 255, 255, 255, 255 };
    SDL_Surface* surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);
    if (!surf)
    {
        std::cerr << "TextRendererComponent::UpdateTexture - Failed to render text: " << SDL_GetError() << "\n";
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
    SDL_FreeSurface(surf);

    if (!texture)
    {
        std::cerr << "TextRendererComponent::UpdateTexture - Failed to create texture: " << SDL_GetError() << "\n";
        return;
    }

    m_TextTexture = std::make_shared<Texture2D>(texture);
    m_NeedsUpdate = false;
}
