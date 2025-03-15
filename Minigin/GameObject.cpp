#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include <algorithm>
#include <iostream>
#include "Observer.h"


namespace dae
{
    GameObject::GameObject()
        : m_parent(nullptr), m_isDirty(true), m_worldPosition(0.0f, 0.0f, 0.0f)
    {
    }

    GameObject::~GameObject()
    {
        // Detach from parent if this object is a child
        if (m_parent)
        {
            SetParent(nullptr); // Remove from parent
        }

        // Clear children
        for (auto& child : m_children)
        {
            child->MarkForDeletion();
            child->m_parent = nullptr;
        }
        m_children.clear();

        // Clear components
        m_components.clear();
    }

    void GameObject::Update(float deltaTime)
    {
        // Update components
        for (auto& component : m_components)
        {
            component->Update(deltaTime);
        }

        // Update children
        for (auto& child : m_children)
        {
            child->Update(deltaTime);
        }

        if (m_isDirty)
        {
            UpdateWorldPosition();
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
        // Use the world position for rendering
        const auto& pos = m_worldPosition;

        if (m_texture)
        {
            Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
        }

        // Render components
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
        // Set the local position in the transform
        m_transform.SetPosition(x, y, 0.0f);

        // Update the world position
        UpdateWorldPosition();

        // Mark as dirty to propagate changes to children
        SetDirty();
    }

    glm::vec3& GameObject::GetPosition()
    {
        return m_worldPosition;
    }

    void GameObject::SetDirty(bool dirty)
    {
        m_isDirty = dirty;
        if (dirty)
        {
            // Mark all children as dirty
            for (auto& child : m_children)
            {
                child->SetDirty();
            }
        }
    }

    bool GameObject::IsDirty() const
    {
        return m_isDirty;
    }

    void GameObject::UpdateWorldPosition()
    {
        if (m_parent)
        {
            // Ensure the parent's world position is up-to-date
            if (m_parent->IsDirty())
            {
                m_parent->UpdateWorldPosition();
            }

            // Calculate the world position based on the parent's world position and local position
            m_worldPosition = m_parent->m_worldPosition + m_transform.GetPosition();
        }
        else
        {
            // If there's no parent, the world position is the same as the local position
            m_worldPosition = m_transform.GetPosition();
        }

        // Mark as clean
        m_isDirty = false;
    }

    void GameObject::SetParent(GameObject* parent)
    {
        if (parent == this || IsChild(parent))
        {
            return; // Invalid parent
        }

        // Store the current world position before changing parents
        glm::vec3 oldWorldPosition = m_worldPosition;

        // Remove from current parent
        if (m_parent)
        {
            auto it = std::find(m_parent->m_children.begin(), m_parent->m_children.end(), this);
            if (it != m_parent->m_children.end())
            {
                m_parent->m_children.erase(it);
            }
        }

        // Set new parent
        m_parent = parent;

        // Add to new parent's children
        if (m_parent)
        {
            m_parent->m_children.push_back(this);

            // Ensure the parent's world position is up-to-date
            if (m_parent->IsDirty())
            {
                m_parent->UpdateWorldPosition();
            }

            // Calculate the new local position relative to the new parent
            glm::vec3 newLocalPosition = oldWorldPosition - m_parent->m_worldPosition;
            m_transform.SetPosition(newLocalPosition.x, newLocalPosition.y, newLocalPosition.z);
        }
        else
        {
            // If there's no parent, the local position is the same as the world position
            m_transform.SetPosition(oldWorldPosition.x, oldWorldPosition.y, oldWorldPosition.z);
        }

        // Mark as dirty to update world position in the next frame
        SetDirty();
    }

    bool GameObject::IsChild(GameObject* potentialChild) const
    {
        for (const auto& child : m_children)
        {
            if (child == potentialChild || child->IsChild(potentialChild))
            {
                return true;
            }
        }
        return false;
    }

    const std::vector<GameObject*>& GameObject::GetChildren() const
    {
        return m_children;
    }

    GameObject* GameObject::GetParent() const
    {
        return m_parent;
    }

    
}