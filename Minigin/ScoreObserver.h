#pragma once
#include "Observer.h"
#include "Event.h"  // Ensure this includes make_sdbm_hash
#include "GameObject.h"
#include "TextRendererComponent.h"
#include <iostream>
#include <any>
namespace dae {
    class ScoreObserver : public Observer {
    public:
        explicit ScoreObserver(std::shared_ptr<dae::GameObject> textObject)
            : m_TextObject(std::move(textObject)), m_LastScore(-1) {
        }

        void Notify(Event event, dae::GameObject* object) override {
            if (!object || !m_TextObject) return;

            if (event.id == make_sdbm_hash("ScoreChanged")) {
                if (!event.args.empty()) {
                    int newScore = std::any_cast<int>(event.args[0]);
                    if (newScore != m_LastScore) { // Only update if the score changed
                        HandleScoreChanged(newScore);
                        m_LastScore = newScore;
                    }
                }
            }
        }

    private:
        void HandleScoreChanged(int newScore) {
            auto textRenderer = m_TextObject->GetComponent<dae::TextRendererComponent>();
            if (textRenderer) {
                textRenderer->SetText(std::to_string(newScore));
                //std::cout << "Updated score display to: " << newScore << "\n";
            }
        }

        std::shared_ptr<dae::GameObject> m_TextObject;
        int m_LastScore; // Store last known score to prevent unnecessary updates
    };
}
