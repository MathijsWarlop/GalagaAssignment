#include "Component.h"
#include "GameObject.h"

dae::Component::Component(GameObject* pOwner)
    : m_pOwner(pOwner)
{
}