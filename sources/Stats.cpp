#include "Stats.h"

Stats& Stats::getInstance() {
    static Stats instance;
    return instance;
}

void Stats::recordRaceResult(const std::string& teamName, const std::string& driverName, const int position) {
    if (position == 1) {
        teamStats[teamName].championshipWins++;
        driverStats[driverName].raceWins++;
    }
    if (position <= 3) {
        teamStats[teamName].podiums++;
        driverStats[driverName].podiums++;
    }
}

void Stats::recordConstructorChampion(const std::string& teamName) {
    teamStats[teamName].constructorChampionships++;
}

void Stats::recordDriverChampion(const std::string& driverName) {
    driverStats[driverName].driverChampionships++;
}

TeamStats Stats::getTeamStats(const std::string& teamName) const {
    const auto it = teamStats.find(teamName);
    return it != teamStats.end() ? it->second : TeamStats{};
}

DriverStats Stats::getDriverStats(const std::string& driverName) const {
    const auto it = driverStats.find(driverName);
    return it != driverStats.end() ? it->second : DriverStats{};
}