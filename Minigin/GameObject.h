#pragma once

#include <memory>
#include <vector>
#include <string>
#include <type_traits>
#include "Transform.h"
#include "BaseComponent.h"
#include "Texture2D.h"
#include "Observer.h"  // Ensure this is included before using Observer
#include "Event.h"
#include <iostream>

namespace dae
{
    class GameObject final
    {
    public:
        GameObject();
        virtual ~GameObject();

        GameObject(const GameObject& other) = delete;
        GameObject(GameObject&& other) = delete;
        GameObject& operator=(const GameObject& other) = delete;
        GameObject& operator=(GameObject&& other) = delete;

        virtual void Update(float deltaTime);
        virtual void FixedUpdate(float fixedTimeStep);
        void LateUpdate();  // Function to clean up deleted components
        virtual void Render() const;

        void SetTexture(const std::string& filename);
        void SetPosition(float x, float y);
        glm::vec3& GetPosition();

        // Dirty flag functionality
        void SetDirty(bool dirty = true);
        bool IsDirty() const;
        void UpdateWorldPosition();

        template <typename T, typename... Args>
        T* AddComponent(Args&&... args)
        {
            static_assert(std::is_base_of<BaseComponent, T>::value, "T must inherit from BaseComponent");

            auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
            //std::cout << "Adding component: " << typeid(T).name() << "\n";

            auto rawPtr = component.get();
            m_components.push_back(std::move(component));

            return rawPtr;
        }


        template <typename T>
        void RemoveComponent() // All components of type T
        {
            static_assert(std::is_base_of<BaseComponent, T>::value, "T must inherit from BaseComponent");

            auto it = std::remove_if(m_components.begin(), m_components.end(),
                [](const std::unique_ptr<BaseComponent>& component) {
                    return dynamic_cast<T*>(component.get()) != nullptr;
                });

            if (it != m_components.end())
            {
                m_components.erase(it, m_components.end());
            }
        }

        bool UnregisterComponent(BaseComponent* component) // Specific component
        {
            auto it = std::remove_if(m_components.begin(), m_components.end(),
                [&](const std::unique_ptr<BaseComponent>& comp) {
                    return comp.get() == component;
                });

            if (it != m_components.end())
            {
                m_components.erase(it, m_components.end());
                return true;
            }
            return false;
        }

        template <typename T>
        T* GetComponent()
        {
            static_assert(std::is_base_of<BaseComponent, T>::value, "T must inherit from BaseComponent");

            for (auto& component : m_components)
            {
                if (auto ptr = dynamic_cast<T*>(component.get()))
                {
                    return ptr;
                }
            }
            return nullptr;
        }

        template <typename T>
        bool HasComponent() const
        {
            static_assert(std::is_base_of<BaseComponent, T>::value, "T must inherit from BaseComponent");

            for (const auto& component : m_components)
            {
                if (dynamic_cast<T*>(component.get()))
                {
                    return true;
                }
            }
            return false;
        }

        void MarkForDeletion() { m_MarkedForDeletion = true; }
        bool IsMarkedForDeletion() const { return m_MarkedForDeletion; }
        void RemoveMarkedComponents(); // Function to remove components marked for deletion

        // Scene graph functionality
        bool IsChild(GameObject* potentialChild) const;
        const std::vector<GameObject*>& GetChildren() const;
        GameObject* GetParent() const;
        void SetParent(GameObject* parent);


        // Observer Pattern
        void RegisterObserver(std::shared_ptr<Observer> observer) {
            //std::cout << "Observer registering!\n";
            m_Observers.push_back(observer);
            //std::cout << "Observers registered: " << m_Observers.size() << "\n";
        }

        void UnregisterObserver(std::shared_ptr<Observer> observer) {
            m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), observer), m_Observers.end());
        }

        void NotifyObservers(const Event& event) {
            //std::cout << "Observers count before notifying: " << m_Observers.size() << "\n";
            for (auto observer : m_Observers) {
                //std::cout << "observer notified \n";
                if (observer) observer->Notify(event, this);
            }
        }

    private:
        std::vector<std::unique_ptr<BaseComponent>> m_components;
        Transform m_transform{};
        std::shared_ptr<Texture2D> m_texture;
        bool m_MarkedForDeletion{ false };  // Flag to track if object should be deleted

        // Scene graph members
        GameObject* m_parent;                  // Pointer to the parent GameObject
        std::vector<GameObject*> m_children;   // List of child GameObjects

        // Dirty flag functionality
        bool m_isDirty{ true };                // Indicates if the position has changed
        glm::vec3 m_worldPosition{ 0.0f };    // Cached world position

        std::vector<std::shared_ptr<Observer>> m_Observers;


    };
}