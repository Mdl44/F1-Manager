#ifndef RACEWEEKEND_H
#define RACEWEEKEND_H
#include <vector>
#include "Team.h"

class RaceWeekend {
    std::string name;
    int laps;
    int reference_time;
    std::vector<std::pair<Driver*, long long>> quali_results;
    std::vector<std::pair<Driver*, long long>> race_results;


public:
    RaceWeekend(std::string name, int laps, int reference_time);
    ~RaceWeekend();
    RaceWeekend(const RaceWeekend& other);
    RaceWeekend& operator=(const RaceWeekend& other);
    static void printResults(std::ostream& os, const std::vector<std::pair<Driver*, long long>>& results, bool isRace) ;
    std::vector<std::pair<Driver*, long long>> race();
    void quali(const std::vector<std::pair<Driver*, int>>& drivers);
    [[nodiscard]] const std::string& get_name() const;
    friend std::ostream& operator<<(std::ostream& os, const RaceWeekend& weekend);
};
#endif