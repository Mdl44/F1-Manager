#ifndef RACEWEEKEND_H
#define RACEWEEKEND_H
#include <vector>
#include "WeatherCondition.h"
#include <memory>
#include "Team.h"

class RaceWeekend {
    std::string name;
    int laps;
    int reference_time;
    bool rain;
    bool night_race;
    std::vector<std::pair<Driver*, long long>> quali_results;
    std::vector<std::pair<Driver*, long long>> race_results;
    std::unique_ptr<WeatherCondition> quali_weather;
    std::unique_ptr<WeatherCondition> race_weather;
    std::vector<Team*> teams;
    std::vector<std::tuple<Driver*, long long, int>> lap_times;
    //void print_pre_weather_stats() const;
    //void print_post_weather_stats() const;
    void apply_weather_effects(const std::unique_ptr<WeatherCondition>& weather) const;
    void remove_weather_effects(const std::unique_ptr<WeatherCondition>& weather) const;
    std::pair<double, double> calculate_performance_factors(const Driver* driver) const;
    void setup_weather(); 


public:
    RaceWeekend(std::string name, int laps, int reference_time, bool rain, bool night_race);
    ~RaceWeekend();
    RaceWeekend(const RaceWeekend& other);
    RaceWeekend& operator=(RaceWeekend other);
    static void printResults(std::ostream& os, const std::vector<std::pair<Driver*, long long>>& results, bool isRace) ;
    std::vector<std::pair<Driver*, long long>> race();
    void quali(const std::vector<std::pair<Driver*, int>>& drivers);
    [[nodiscard]] const std::string& get_name() const;
    friend std::ostream& operator<<(std::ostream& os, const RaceWeekend& weekend);
    friend void swap(RaceWeekend& first, RaceWeekend& second) noexcept;
    [[nodiscard]] const std::vector<std::pair<Driver*, long long>>& get_quali_results() const;

    void set_teams(const std::vector<Team*>& race_teams);

    void record_lap_time(Driver* driver, long long time, int lap);
    [[nodiscard]] const std::vector<std::tuple<Driver*, long long, int>>& get_lap_times() const;
    
};
#endif