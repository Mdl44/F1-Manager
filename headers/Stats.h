#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include <unordered_map>

struct TeamStats {
    int championshipWins = 0;
    int podiums = 0;
    int constructorChampionships = 0;
};

struct DriverStats {
    int raceWins = 0;
    int podiums = 0;
    int driverChampionships = 0;
};


class Stats {
    Stats() = default;
    std::unordered_map<std::string, TeamStats> teamStats;
    std::unordered_map<std::string, DriverStats> driverStats;

public:
    Stats(const Stats&) = delete;
    Stats& operator=(const Stats&) = delete;

    static Stats& getInstance();
    void recordRaceResult(const std::string& teamName, const std::string& driverName, int position);
    void recordConstructorChampion(const std::string& teamName);
    void recordDriverChampion(const std::string& driverName);
    TeamStats getTeamStats(const std::string& teamName) const;
    DriverStats getDriverStats(const std::string& driverName) const;
};

#endif