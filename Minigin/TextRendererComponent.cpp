#include "TextRendererComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include <iostream>

dae::TextRendererComponent::TextRendererComponent(GameObject* pOwner, const std::string& text, float x, float y)
    : BaseComponent(pOwner), m_Text(text), m_X(x), m_Y(y)  // Initialize members
{
    auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
    m_pTextObject = std::make_shared<dae::TextObject>(m_Text, font);
    m_pTextObject->SetPosition(m_X, m_Y);
}

void dae::TextRendererComponent::Render() const
{
    if (m_pTextObject)
    {
        m_pTextObject->Render(); // Use the existing TextObject instead of recreating one
        //std::cout << "rendering";
    }
}

void dae::TextRendererComponent::Update()
{
    if (m_pTextObject)
    {
        //std::cout << "TextRendererComponent::Update() called!" << std::endl;
        m_pTextObject->Update();  // This is the missing call
    }
}


void dae::TextRendererComponent::SetText(const std::string& text)
{
    if (m_pTextObject)
    {
        m_pTextObject->SetText(text);
        //std::cout << "Updating text to: " << text << std::endl;
    }
}
