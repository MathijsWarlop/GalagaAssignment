#pragma once

namespace dae
{
    class GameObject;

    class BaseComponent
    {
    public:
        // Constructor and Destructor
        BaseComponent(GameObject* pOwner);
        virtual ~BaseComponent() = default;

        // Disable copying and moving
        BaseComponent(const BaseComponent& other) = delete;
        BaseComponent(BaseComponent&& other) = delete;
        BaseComponent& operator=(const BaseComponent& other) = delete;
        BaseComponent& operator=(BaseComponent&& other) = delete;

        // Core methods
        virtual void Update(float ) {  } 
        virtual void FixedUpdate(float fixedTimeStep) { fixedTimeStep; }
        virtual void Render() const {}

        // Deletion methods
        void MarkForDeletion() { m_MarkedForDeletion = true; }
        bool IsMarkedForDeletion() const { return m_MarkedForDeletion; }

        // Get the owner GameObject
        GameObject* GetOwner() const { return m_pOwner; }

    protected:
        GameObject* m_pOwner; // Pointer to the GameObject this component is attached to
        bool m_MarkedForDeletion{ false };  // Flag to track if it should be deleted
    };
}
