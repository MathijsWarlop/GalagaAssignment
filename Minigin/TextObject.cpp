#include <stdexcept>
#include <SDL_ttf.h>
#include "TextObject.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

#include <iostream>

dae::TextObject::TextObject(const std::string& text, std::shared_ptr<Font> font) 
	: m_needsUpdate(true), m_text(text), m_font(std::move(font)), m_textTexture(nullptr)
{ }

void dae::TextObject::Update()
{
    if (m_needsUpdate)
    {
        //std::cout << "TextObject::Update() - Updating texture for: " << m_text << std::endl;

        const SDL_Color color = { 255,255,255,255 };
        const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), color);
        if (!surf)
        {
            std::cerr << "Render text failed: " << SDL_GetError() << std::endl;
            return;
        }
        auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
        if (!texture)
        {
            std::cerr << "Create text texture failed: " << SDL_GetError() << std::endl;
            return;
        }
        SDL_FreeSurface(surf);
        m_textTexture = std::make_shared<Texture2D>(texture);
        m_needsUpdate = false;
    }
}


void dae::TextObject::Render() const
{
	if (m_textTexture != nullptr)
	{
		const auto& pos = m_transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_textTexture, pos.x, pos.y);
	}
}

// This implementation uses the "dirty flag" pattern
void dae::TextObject::SetText(const std::string& text)
{
    m_text = text;
    m_needsUpdate = true; // This should trigger Update()

//    std::cout << "SetText called: " << m_text << ", m_needsUpdate set to " << m_needsUpdate << std::endl;
}



void dae::TextObject::SetPosition(const float x, const float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}


