#ifndef SEASON_H
#define SEASON_H
#include <map>
#include <vector>
#include "Team.h"
#include "RaceWeekend.h"
#include <random>

class Season {
    std::vector<Team*> teams;
    std::map<std::string, int> driver_points;
    std::map<std::string, int> team_points;
    int races;
    int current_race = 1;

public:
    explicit Season(const std::vector<Team*>& team_list, int total_races = 24);
    Season(const Season&);
    Season& operator=(const Season&);
    ~Season();

    void race(RaceWeekend& weekend);
    void standings(const std::vector<std::pair<Driver*, long long>>& race_results);
    static void printStandings(std::ostream& os, const std::vector<std::pair<std::string, int>>& standings, const std::string& title, int lungime) ;

    void update_team_performance();
    static int calculate_combined_rating(const Team* team, const Driver* driver);
    friend std::ostream& operator<<(std::ostream& os, const Season& season);
};
#endif
