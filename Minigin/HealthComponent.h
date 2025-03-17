#pragma once
#include "BaseComponent.h"
#include "event.h"
#include "GameObject.h"

namespace dae
{
    class HealthComponent : public BaseComponent {
    public:
        explicit HealthComponent(GameObject* owner, int maxHealth)
            : BaseComponent(owner), m_Health(maxHealth), m_MaxHealth(maxHealth) {
        }

        void TakeDamage(int damage) {
            if (m_Health <= 0) return; // Already dead, ignore further damage

            m_Health -= damage;

            // Notify observers of health change
            Event healthEvent(make_sdbm_hash("HealthChanged"));
            healthEvent.AddArgument(m_Health);
            GetOwner()->NotifyObservers(healthEvent);

            if (m_Health <= 0) {
                m_Health = 0;
                NotifyDeath();
            }
        }



        int GetHealth() const { return m_Health; }
    private:
        int m_Health;
        int m_MaxHealth;

        void NotifyDeath() {
            Event e(make_sdbm_hash("ShipDestroyed"));
            GetOwner()->NotifyObservers(e);  
        }
    };
}