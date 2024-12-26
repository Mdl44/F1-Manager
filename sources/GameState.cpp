#include "GameState.h"

GameState& GameState::getInstance() {
    static GameState instance;
    return instance;
}

void GameState::recordRaceResult(const std::string& teamName, const std::string& driverName, const int position) {
    if (position == 1) {
        teamStats[teamName].championshipWins++;
        driverStats[driverName].raceWins++;
    }
    if (position <= 3) {
        teamStats[teamName].podiums++;
        driverStats[driverName].podiums++;
    }
}

void GameState::recordConstructorChampion(const std::string& teamName) {
    teamStats[teamName].constructorChampionships++;
}

void GameState::recordDriverChampion(const std::string& driverName) {
    driverStats[driverName].driverChampionships++;
}

TeamStats GameState::getTeamStats(const std::string& teamName) const {
    const auto it = teamStats.find(teamName);
    return it != teamStats.end() ? it->second : TeamStats{};
}

DriverStats GameState::getDriverStats(const std::string& driverName) const {
    const auto it = driverStats.find(driverName);
    return it != driverStats.end() ? it->second : DriverStats{};
}

void GameState::incrementSeason() {
    currentSeason++;
}

int GameState::getCurrentSeason() const {
    return currentSeason;
}