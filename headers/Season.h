#ifndef SEASON_H
#define SEASON_H
#include <map>
#include <vector>
#include "Team.h"
#include "RaceWeekend.h"

class Season {
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
    void standings(const std::vector<std::pair<Driver*, long long>>& race_results);

    void update_team_performance();
    static int calculate_combined_rating(const Team* team, const Driver* driver);
    void display_standings();
};
#endif