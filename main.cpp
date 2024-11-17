#include <fstream>
#include <memory>
#include <iostream>
#include <vector>
#include "Player.h"
#include "Season.h"

int main() {
    std::vector<std::vector<int>> car_stats;
    std::ifstream car_file("Date masini.txt");
    if (!car_file) {
        return 1;
    }
    int aero, power, durability, chassis, pos;
    while (car_file >> aero >> power >> durability >> chassis >> pos) {
        car_stats.push_back({aero, power, durability, chassis});
    }

    std::vector<RaceWeekend> circuits;
    std::ifstream circuit_file("circuite.txt");
    if (!circuit_file) {
        return 1;
    }
    std::string name;
    int laps, reference_time;
    while (std::getline(circuit_file, name)) {
        if (!(circuit_file >> reference_time >> laps)) break;
        circuit_file.ignore();
        circuits.emplace_back(name, laps, reference_time);
    }

    std::vector<std::unique_ptr<Team>> teams;
    std::ifstream team_file("piloti.txt");
    if (!team_file) {
        return 1;
    }
    int num_teams;
    team_file >> num_teams;
    team_file.ignore();
    for (size_t i = 0; i < static_cast<size_t>(num_teams) && i < car_stats.size(); ++i) {
        std::string team_name;
        std::getline(team_file, team_name);

        std::string driver1_name, driver2_name;
        int driver1_stats[4], driver2_stats[4];

        std::getline(team_file, driver1_name);
        for (int& stat : driver1_stats) team_file >> stat;
        team_file.ignore();

        std::getline(team_file, driver2_name);
        for (int& stat : driver2_stats) team_file >> stat;
        team_file.ignore();

        auto car1 = std::make_unique<Car>(car_stats[i][0], car_stats[i][1], car_stats[i][2], car_stats[i][3], nullptr);
        auto car2 = std::make_unique<Car>(car_stats[i][0], car_stats[i][1], car_stats[i][2], car_stats[i][3], nullptr);

        auto driver1 = std::make_unique<Driver>(driver1_name, driver1_stats[0], driver1_stats[1], driver1_stats[2], driver1_stats[3], car1.get(), nullptr);
        auto driver2 = std::make_unique<Driver>(driver2_name, driver2_stats[0], driver2_stats[1], driver2_stats[2], driver2_stats[3], car2.get(), nullptr);

        teams.push_back(std::make_unique<Team>(team_name, car1.release(), car2.release(), driver1.release(), driver2.release(), i + 1));
    }

    Team* userTeam = teams[1].get();
    Player player(userTeam);

    std::vector<Team*> teamPtrs;
    teamPtrs.reserve(teams.size());
    for (const auto& team : teams) {
        teamPtrs.push_back(team.get());
    }
    Season season(teamPtrs);

    for (size_t i = 0; i < circuits.size() && i < 24; ++i) {
        season.race(circuits[i]);

        if (i == 0) {
            Driver* firstDriver = userTeam->get_driver2();
            Driver* secondDriver = teams[0]->get_driver2();
            player.swap_try(firstDriver, secondDriver, *teams[0]);
        }
    }
    return 0;
}