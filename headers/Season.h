#ifndef SEASON_H
#define SEASON_H
#include <map>
#include <vector>
#include "Team.h"
#include "RaceWeekend.h"
#include <random>

#include "Exceptions.h"
#include "Stats.h"
#include "PerformanceTracker.h"

class Season {
    std::vector<Team*> teams;
    std::map<std::string, int> driver_points;
    std::map<std::string, int> team_points;
    int races;
    int current_race = 1;

    PerformanceTracker<int> car_ratings{"Car Ratings"};
    PerformanceTracker<int> driver_ratings{"Driver Ratings"};
    PerformanceTracker<long long> quali_performances{"Qualifying"};
    PerformanceTracker<long long> race_performances{"Race"};

    std::vector<std::tuple<Driver*, long long, int>> lap_times;

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
    void recordSeasonChampions();
    

    template<typename T>
    std::tuple<T, long long, int> find_fastest_lap(
            const std::vector<std::tuple<T, long long, int>>& laps) {
        if (laps.empty()) {
            throw RaceWeekendException("No lap times recorded");
        }
        return *std::min_element(laps.begin(), laps.end(),
            [](const auto& a, const auto& b) {
                return std::get<1>(a) < std::get<1>(b);
            });
    }
};
#endif