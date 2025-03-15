#pragma once
#include "Observer.h"
#include "Event.h"  // Make sure this includes make_sdbm_hash
#include "GameObject.h"
#include <iostream>
#include <any>

class HealthObserver : public Observer {
public:
    void Notify(Event event, dae::GameObject* object) override {
        //std::cout <<"Notify in healthobserver called \n";
        if (!object) return;

        if (event.id == make_sdbm_hash("ShipDestroyed")) {
            HandleDeath(object);
        }
        else if (event.id == make_sdbm_hash("HealthChanged")) {
            if (!event.args.empty()) {
                int newHealth = std::any_cast<int>(event.args[0]);
                HandleHealthChanged(object, newHealth);
            }
        }
    }

private:
    void HandleHealthChanged(dae::GameObject* object, int newHealth) {
        std::cout << "Ship now has " << newHealth << " HP.\n";
        object;
    }

    void HandleDeath(dae::GameObject* object) {
        object;
        std::cout << "Ship has died!\n";
    }
};
