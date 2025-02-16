#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

namespace dae
{
    GameObject::~GameObject() = default;

    void GameObject::Update()
    {
        for (auto& component : m_components)
        {
            component->Update();
        }
    }

    void GameObject::FixedUpdate(float fixedTimeStep)
    {
        for (auto& component : m_components)
        {
            component->FixedUpdate(fixedTimeStep);
        }
    }

    void GameObject::Render() const
    {
        const auto& pos = m_transform.GetPosition();
        if (m_texture)
        {
            Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
        }

        for (const auto& component : m_components)
        {
            component->Render();
        }
    }

    void GameObject::SetTexture(const std::string& filename)
    {
        m_texture = ResourceManager::GetInstance().LoadTexture(filename);
    }

    void GameObject::SetPosition(float x, float y)
    {
        m_transform.SetPosition(x, y, 0.0f);
    }
}
