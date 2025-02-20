#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include <algorithm>

namespace dae
{
    GameObject::~GameObject()
    {
        // Detach from parent if this object is a child
        if (m_parent)
        {
            m_parent->RemoveChild(this);
        }

        // Clear children
        for (auto& child : m_children)
        {
            child->m_parent = nullptr;
        }
        m_children.clear();

        // Clear components
        m_components.clear();
    }

    void GameObject::Update()
    {
        // Update components
        for (auto& component : m_components)
        {
            component->Update();
        }

        // Update children
        for (auto& child : m_children)
        {
            child->Update();
        }
    }

    void GameObject::FixedUpdate(float fixedTimeStep)
    {
        // Fixed update components
        for (auto& component : m_components)
        {
            component->FixedUpdate(fixedTimeStep);
        }

        // Fixed update children
        for (auto& child : m_children)
        {
            child->FixedUpdate(fixedTimeStep);
        }
    }

    void GameObject::LateUpdate()
    {
        // Late update components
        RemoveMarkedComponents();

        // Late update children
        for (auto& child : m_children)
        {
            child->LateUpdate();
        }
    }

    void GameObject::RemoveMarkedComponents()
    {
        m_components.erase(std::remove_if(m_components.begin(), m_components.end(),
            [](const std::unique_ptr<BaseComponent>& comp) {
                return comp->IsMarkedForDeletion();
            }), m_components.end());
    }

    void GameObject::Render() const
    {
        // Render this object
        const auto& pos = m_transform.GetPosition();
        if (m_texture)
        {
            Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
        }

        for (const auto& component : m_components)
        {
            component->Render();
        }

        // Render children
        for (const auto& child : m_children)
        {
            child->Render();
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

    // Scene graph functionality
    void GameObject::AddChild(GameObject* child)
    {
        if (child && child != this && !child->m_parent)
        {
            // Detach from previous parent if any
            if (child->m_parent)
            {
                child->m_parent->RemoveChild(child);
            }

            // Attach to this parent
            m_children.push_back(child);
            child->m_parent = this;
        }
    }

    void GameObject::RemoveChild(GameObject* child)
    {
        if (child)
        {
            auto it = std::find(m_children.begin(), m_children.end(), child);
            if (it != m_children.end())
            {
                (*it)->m_parent = nullptr;
                m_children.erase(it);
            }
        }
    }

    const std::vector<GameObject*>& GameObject::GetChildren() const
    {
        return m_children;
    }

    GameObject* GameObject::GetParent() const
    {
        return m_parent;
    }

    void GameObject::SetParent(GameObject* parent)
    {
        if (parent)
        {
            parent->AddChild(this);
        }
        else if (m_parent)
        {
            m_parent->RemoveChild(this);
        }
    }
}