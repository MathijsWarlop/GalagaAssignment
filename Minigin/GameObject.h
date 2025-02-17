#pragma once

#include <memory>
#include <vector>
#include <string>
#include <type_traits>
#include "Transform.h"
#include "BaseComponent.h"
#include "Texture2D.h"



namespace dae
{
    class GameObject final
    {
    public:
        GameObject() = default;
        virtual ~GameObject();

        GameObject(const GameObject& other) = delete;
        GameObject(GameObject&& other) = delete;
        GameObject& operator=(const GameObject& other) = delete;
        GameObject& operator=(GameObject&& other) = delete;

        virtual void Update();
        virtual void FixedUpdate(float fixedTimeStep);
        void LateUpdate();  //function to clean up deleted components
        virtual void Render() const;

        void SetTexture(const std::string& filename);
        void SetPosition(float x, float y);

        template <typename T, typename... Args>
        T* AddComponent(Args&&... args)
        {
            static_assert(std::is_base_of<BaseComponent, T>::value, "T must inherit from BaseComponent");

            auto component = std::make_unique<T>(this, std::forward<Args>(args)...);
            T* rawPtr = component.get();
            m_components.push_back(std::move(component));

            return rawPtr;
        }

        template <typename T>
        void RemoveComponent() //all components of type T
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

        bool UnregisterComponent(BaseComponent* component) //specific component
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

    private:
        std::vector<std::unique_ptr<BaseComponent>> m_components;
        Transform m_transform{};
        std::shared_ptr<Texture2D> m_texture;
        bool m_MarkedForDeletion{ false };  // Flag to track if object should be deleted
    };
}
