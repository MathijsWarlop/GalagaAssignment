#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include <algorithm>
#include <iostream>
namespace dae
{
    GameObject::GameObject()
        : m_parent(nullptr), m_isDirty(true), m_worldPosition(0.0f, 0.0f, 0.0f)
    {
    }

    GameObject::~GameObject() //WRONG
    {
        // Detach from parent if this object is a child
        if (m_parent)
        {
            m_parent->RemoveChild(this); //parents need to know its deleted
        }

        // Clear children
        for (auto& child : m_children)
        {
            child->m_parent = nullptr; //setparent? // childeren needs to be deleted, branch must be deleted
        }
        m_children.clear();

        // Clear components
        m_components.clear();
    }

    void GameObject::Update( float deltaTime)
    {
       
        //std::cout << "Update() triggered" << std::endl;
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
        const auto& pos = m_worldPosition; // Or use m_transform.GetPosition() if you're using it
        //std::cout << "Rendering at position: (" << pos.x << ", " << pos.y << ")" << std::endl;

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
        m_transform.SetPosition(x, y, 0.0f);
        UpdateWorldPosition();
        //std::cout << x << "  " << y << std::endl;
        SetDirty(); // Mark as dirty when position changes

    }
    glm::vec3& GameObject::GetPosition()
    {
        return m_worldPosition;

    }
    // Dirty flag functionality
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
        //std::cout << "UpdateWorldPosition()" << std::endl;
        if (m_parent)
        {
            //std::cout << "parent" << std::endl;
            // Calculate world position based on parent's world position
            m_worldPosition = m_parent->m_worldPosition + m_transform.GetPosition();
        }
        else
        {
            // No parent, so world position is the same as local position
            m_worldPosition = m_transform.GetPosition();
            //std::cout << m_transform.GetPosition().x << "  " << m_transform.GetPosition().y << std::endl;
            //std::cout << m_worldPosition.x << "  " << m_worldPosition.y << std::endl;
        }

        // Mark as clean
        m_isDirty = false;
        
    }

    // Scene graph functionality
    void GameObject::AddChild(GameObject* child)
    {
        if (child && child != this) // && !child->m_parent)
        {
            // Detach from previous parent if any
            if (child->m_parent)
            {
                child->m_parent->RemoveChild(child);
            }

            // Attach to this parent
            m_children.push_back(child);
            child->m_parent = this;
            child->SetDirty(); // Mark child as dirty when parent changes
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
                (*it)->SetDirty(); // Mark child as dirty when parent changes
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
        SetDirty(); // Mark as dirty when parent changes
    }
}