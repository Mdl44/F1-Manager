#ifndef RACEWEEKEND_H
#define RACEWEEKEND_H
#include <vector>
#include "Driver.h"

class RaceWeekend {
    std::string name;
    int laps;
    int reference_time;
    std::vector<std::pair<Driver*, double>> quali_results;
    std::vector<std::pair<Driver*, long long>> race_results;

public:
    RaceWeekend(std::string name, int laps, int reference_time);
    std::vector<std::pair<Driver*, long long>> race();
    void quali(const std::vector<Driver*>& drivers);
    void display_quali() const;
    void display_race() const;

    std::string get_name() const;
    const std::vector<std::pair<Driver*, double>>& get_quali_results() const;
};

#endif