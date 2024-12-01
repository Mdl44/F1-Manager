#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <vector>
#include <memory>
#include "Season.h"

class GameManager {
    std::vector<std::unique_ptr<Team>> teams;
    std::vector<std::unique_ptr<RaceWeekend>> circuits;
    Team* my_team;

public:
    GameManager();
    bool initialize();
    [[nodiscard]] Team* get_my_team() const;
    std::vector<std::unique_ptr<Team>>& get_teams();
    std::vector<std::unique_ptr<RaceWeekend>>& get_circuits();
};

#endif