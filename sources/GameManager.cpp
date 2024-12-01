#include "GameManager.h"
#include <fstream>
#include <iostream>

GameManager::GameManager() : my_team(nullptr) {}

bool GameManager::initialize() {
    std::vector<std::vector<int>> car_stats;
    std::ifstream car_file("date_masini.txt");
    if (!car_file) {
        std::cerr << "Failed to open car data file\n";
        return false;
    }

    int aero, power, durability, chassis, pos;
    while (car_file >> aero >> power >> durability >> chassis >> pos) {
        car_stats.push_back({aero, power, durability, chassis});
    }

    std::ifstream circuit_file("circuite.txt");
    if (!circuit_file) {
        std::cerr << "Failed to open circuit data file\n";
        return false;
    }

    std::string name;
    int laps, reference_time;
    while (std::getline(circuit_file, name)) {
        if (!(circuit_file >> reference_time >> laps)) break;
        circuit_file.ignore();
        circuits.emplace_back(std::make_unique<RaceWeekend>(name, laps, reference_time));
    }

    std::ifstream team_file("piloti.txt");
    if (!team_file) {
        std::cerr << "Failed to open pilot data file\n";
        return false;
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
        for (int& stat : driver1_stats) {
            team_file >> stat;
        }
        team_file.ignore();

        std::getline(team_file, driver2_name);
        for (int& stat : driver2_stats) {
            team_file >> stat;
        }
        team_file.ignore();

        auto car1 = std::make_unique<Car>(car_stats[i][0], car_stats[i][1], car_stats[i][2], car_stats[i][3]);
        auto car2 = std::make_unique<Car>(car_stats[i][0], car_stats[i][1], car_stats[i][2], car_stats[i][3]);

        auto driver1 = std::make_unique<Driver>(driver1_name, driver1_stats[0], driver1_stats[1], driver1_stats[2], driver1_stats[3]);
        auto driver2 = std::make_unique<Driver>(driver2_name, driver2_stats[0], driver2_stats[1], driver2_stats[2], driver2_stats[3]);

        teams.push_back(std::make_unique<Team>(team_name, std::move(car1), std::move(car2), std::move(driver1), std::move(driver2), i + 1));
    }

    std::cout << "Select your team:\n";
    for (size_t i = 0; i < teams.size(); ++i) {
        std::cout << i + 1 << ". " << teams[i]->get_name() << "\n";
    }

    size_t choice;
    if (!(std::cin >> choice) || choice < 1 || choice > teams.size()) {
        std::cerr << "Invalid team selection!\n";
        return false;
    }

    my_team = teams[choice - 1].get();
    return true;
}

Team* GameManager::get_my_team() const {
    return my_team;
}

std::vector<std::unique_ptr<Team>>& GameManager::get_teams() {
    return teams;
}

std::vector<std::unique_ptr<RaceWeekend>>& GameManager::get_circuits() {
    return circuits;
}