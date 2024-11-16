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
    int current_race = 0;
    static std::string pad_right(const std::string& str, size_t width) {
        if (str.length() >= width) return str;
        return str + std::string(width - str.length() + 1, ' ');
    }

public:
    Season(const std::vector<Team*>& teamList, int totalRaces = 24);
    Season(const Season&) = delete;
    Season& operator=(const Season&) = delete;

    void race(RaceWeekend& weekend);
    void standings(const std::vector<std::pair<Driver *, long long>> &raceResults);
    void display_standings();
    std::vector<std::pair<std::string, int>> get_driver_standings() const;
    std::vector<std::pair<std::string, int>> get_team_standings() const;
};

#endif