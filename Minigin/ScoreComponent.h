#pragma once
#include "BaseComponent.h"
#include "event.h"
#include "GameObject.h"

namespace dae {
    class ScoreComponent : public BaseComponent {
    public:
        explicit ScoreComponent(GameObject* owner, int initialScore = 0)
            : BaseComponent(owner), m_Score(initialScore) {
        }

        void AddPoints(int points) {
            m_Score += points;

            // Notify observers of score change
            Event scoreEvent(make_sdbm_hash("ScoreChanged"));
            scoreEvent.AddArgument(m_Score);
            GetOwner()->NotifyObservers(scoreEvent);

            //std::cout << "ScoreComponent: Score updated to " << m_Score << "\n";
        }

        int GetScore() const { return m_Score; }

    private:
        int m_Score;
    };
}
