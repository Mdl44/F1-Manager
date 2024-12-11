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
    bool rain{};
    bool night_race{};
    std::vector<std::pair<Driver*, long long>> quali_results;
    std::vector<std::pair<Driver*, long long>> race_results;
    std::unique_ptr<WeatherCondition> quali_weather;
    std::unique_ptr<WeatherCondition> race_weather;
    std::vector<Team*> teams;


public:
    RaceWeekend(std::string name, int laps, int reference_time, bool rain, bool night_race);
    ~RaceWeekend();
    RaceWeekend(const RaceWeekend& other);
    RaceWeekend& operator=(const RaceWeekend& other);
    static void printResults(std::ostream& os, const std::vector<std::pair<Driver*, long long>>& results, bool isRace) ;
    std::vector<std::pair<Driver*, long long>> race();
    void quali(const std::vector<std::pair<Driver*, int>>& drivers);
    [[nodiscard]] const std::string& get_name() const;
    friend std::ostream& operator<<(std::ostream& os, const RaceWeekend& weekend);


    void set_quali_weather(std::unique_ptr<WeatherCondition> weather);
    void set_race_weather(std::unique_ptr<WeatherCondition> weather);
    [[nodiscard]] bool can_rain() const;
    [[nodiscard]] bool night() const;
    [[nodiscard]] WeatherCondition* get_quali_weather() const;
    [[nodiscard]] WeatherCondition* get_race_weather() const;
};
#endif