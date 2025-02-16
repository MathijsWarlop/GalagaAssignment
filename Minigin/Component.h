#pragma once
#pragma once

namespace dae
{
    class GameObject;

    class Component
    {
    public:
        // Constructor and Destructor
        Component(GameObject* pOwner);
        virtual ~Component() = default;

        // Disable copying and moving
        Component(const Component& other) = delete;
        Component(Component&& other) = delete;
        Component& operator=(const Component& other) = delete;
        Component& operator=(Component&& other) = delete;

        // Core methods
        virtual void Update() {}
        virtual void FixedUpdate(float fixedTimeStep) { fixedTimeStep; }
        virtual void Render() const {}

        // Get the owner GameObject
        GameObject* GetOwner() const { return m_pOwner; }

    protected:
        GameObject* m_pOwner; // Pointer to the GameObject this component is attached to
    };
}