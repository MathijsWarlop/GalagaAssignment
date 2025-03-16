#pragma once
#include "Observer.h"
#include "Event.h"
#include "GameObject.h"
#include "TextRendererComponent.h"
#include <iostream>
#include <any>

class HealthObserver : public Observer {
public:
    explicit HealthObserver(std::shared_ptr<dae::GameObject> textObject)
        : m_TextObject(std::move(textObject)) {
    }

    void Notify(Event event, dae::GameObject* object) override {
        if (!object || !m_TextObject) return;

        auto textRenderer = m_TextObject->GetComponent<dae::TextRendererComponent>();
        if (!textRenderer) return;

        if (event.id == make_sdbm_hash("ShipDestroyed")) {
            textRenderer->SetText("Ship has died!");
        }
        else if (event.id == make_sdbm_hash("HealthChanged")) {
            if (!event.args.empty()) {
                int newHealth = std::any_cast<int>(event.args[0]);
                textRenderer->SetText(std::to_string(newHealth));
            }
        }
    }

private:
    std::shared_ptr<dae::GameObject> m_TextObject; // The object displaying health text
};
