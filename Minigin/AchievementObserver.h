#pragma once
#include "Observer.h"
#include "Event.h"
#include "GameObject.h"
#include <steam_api.h>
#include <iostream>
#include <any>
namespace dae {
    class AchievementObserver : public Observer {
    public:
        void Notify(Event event, dae::GameObject* object) override {
            if (!object) return;

            if (event.id == make_sdbm_hash("ScoreChanged")) {
                if (!event.args.empty()) {
                    int newScore = std::any_cast<int>(event.args[0]);
                    CheckAchievements(newScore);
                }
            }
        }

    private:
        void CheckAchievements(int score) {
            if (!SteamUserStats()) return;

            if (score >= 500) {
                SteamUserStats()->SetAchievement("ACH_WIN_ONE_GAME");  // Spacewar achievement name
                SteamUserStats()->StoreStats();  // Saves achievement progress to Steam
                //std::cout << "Achievement Unlocked: Win After 500 Points!\n";
            }
        }
    };
}
