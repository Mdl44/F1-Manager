#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <vector>
#include <memory>
#include "Season.h"
#include "GameView.h"

class GameManager {
    std::vector<std::unique_ptr<Team>> teams;
    std::vector<std::unique_ptr<RaceWeekend>> circuits;
    Team* my_team;

public:
    GameManager();
    bool initialize(GameView& view);
    [[nodiscard]] Team* get_my_team() const;
    std::vector<std::unique_ptr<Team>>& get_teams();
    std::vector<std::unique_ptr<RaceWeekend>>& get_circuits();

private:
    // Offers circuits dropped from the 2026 calendar as optional bonus rounds.
    void offer_legacy_circuits(GameView& view);
};

#endif