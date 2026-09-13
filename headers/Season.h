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

// Everything a completed race weekend produced, for a GameView to display.
struct RaceOutcome {
    std::string weekend_report;
    std::string fastest_lap_report;
    std::vector<std::string> team_events;
    std::string season_analysis; // empty unless this was the final race of the season
    std::string standings_report;
};

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
    Driver* fastest_lap_driver = nullptr;

public:
    explicit Season(const std::vector<Team*>& team_list, int total_races);
    Season(const Season&);
    Season& operator=(const Season&);
    ~Season();

    RaceOutcome race(RaceWeekend& weekend);
    std::vector<std::string> standings(const std::vector<std::pair<Driver*, long long>>& race_results);
    static void printStandings(std::ostream& os, const std::vector<std::pair<std::string, int>>& standings, const std::string& title, int lungime) ;

    std::vector<std::string> update_team_performance();
    static int calculate_combined_rating(const Team* team, const Driver* driver);
    friend std::ostream& operator<<(std::ostream& os, const Season& season);
    std::vector<std::string> recordSeasonChampions();
    

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