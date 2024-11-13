#ifndef RACEWEEKEND_H
#define RACEWEEKEND_H
#include <iostream>
#include <vector>
#include <string>
#include "Driver.h"

class RaceWeekend {
    std::string name;
    int laps;
    int reference_time;
    std::vector<std::pair<Driver*, double>> quali_results;
    std::vector<std::pair<Driver*, long long>> race_results;
public:
    RaceWeekend(const std::string name, int laps, int reference_time);

    void quali(const std::vector<Driver*>& drivers);
    void display_quali() const;
    void race();
    void display_race() const;

    std::string get_name() const;
    int get_laps() const;
    int get_reference_time() const;
};

#endif
