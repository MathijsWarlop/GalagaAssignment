#pragma once
#include "Event.h"

namespace dae {
    class GameObject;  // Forward declaration instead of #include "GameObject.h"


    class Observer
    {
    public:
        virtual ~Observer() = default;
        virtual void Notify(Event event, dae::GameObject* object) = 0;
    };
}
