#ifndef SEASON_H
#define SEASON_H
#include <map>
#include "Team.h"
#include "RaceWeekend.h"

class Season {
    std::vector<Driver*> drivers;
    std::vector<Team*> teams;
    std::map<std::string, int> driver_points;
    std::map<std::string, int> team_points;
    int races;
    int current_race = 1;

public:
    explicit Season(const std::vector<Team*>& team_list, int total_races = 24);
    Season(const Season&) = delete;
    Season& operator=(const Season&) = delete;

    void race(RaceWeekend& weekend);
    void standings(const std::vector<std::pair<Driver *, long long>> &race_results);

    static void apply_ai_team_upgrades(Team *team);

    static void apply_player_team_upgrades(const Team *team);

    void display_standings();
};

#endif