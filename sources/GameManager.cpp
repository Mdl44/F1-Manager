#include "GameManager.h"
#include <fstream>
#include <iostream>
#include "TopTeam.h"
#include "Exceptions.h"

GameManager::GameManager() : my_team(nullptr) {}

bool GameManager::initialize() {

    std::vector<std::vector<int>> car_stats;
    std::ifstream car_file("date_masini.txt");
    if (!car_file) {
        throw ConfigurationFileException("date_masini.txt");
    }

    int aero, power, durability, chassis;
    while (car_file >> aero >> power >> durability >> chassis) {
        car_stats.push_back({aero, power, durability, chassis});
    }


    std::ifstream circuit_file("circuite.txt");
    if (!circuit_file) {
        throw ConfigurationFileException("circuite.txt");
    }

    std::string name;
    int laps, reference_time;
    bool rain, night_race;
    while (std::getline(circuit_file, name)) {
        if (!(circuit_file >> reference_time >> laps >> night_race >> rain)) break;
        circuit_file.ignore();
        circuits.emplace_back(std::make_unique<RaceWeekend>(name, laps, reference_time, rain, night_race));
    }

    std::ifstream team_file("piloti.txt");
    if (!team_file) {
        throw ConfigurationFileException("piloti.txt");
    }

    int num_teams;
    team_file >> num_teams;
    team_file.ignore();

    for (size_t i = 0; i < static_cast<size_t>(num_teams) && i < car_stats.size(); ++i) {
        std::string team_name;
        int expected_position;
        int dry_bonus, inter_bonus, wet_bonus, night_bonus;
        
        std::getline(team_file, team_name);
        team_file >> expected_position;
        team_file >> dry_bonus >> inter_bonus >> wet_bonus >> night_bonus;
        team_file.ignore();

        std::string driver1_name;
        int driver1_exp, driver1_craft, driver1_aware, driver1_pace;
        int driver1_age, driver1_dry, driver1_inter, driver1_wet;
        
        std::getline(team_file, driver1_name);
        team_file >> driver1_exp >> driver1_craft >> driver1_aware >> driver1_pace
                  >> driver1_age >> driver1_dry >> driver1_inter >> driver1_wet;
        team_file.ignore();

        std::string driver2_name;
        int driver2_exp, driver2_craft, driver2_aware, driver2_pace;
        int driver2_age, driver2_dry, driver2_inter, driver2_wet;
        
        std::getline(team_file, driver2_name);
        team_file >> driver2_exp >> driver2_craft >> driver2_aware >> driver2_pace
                  >> driver2_age >> driver2_dry >> driver2_inter >> driver2_wet;
        team_file.ignore();

        auto car1 = std::make_unique<Car>(car_stats[i][0], car_stats[i][1], car_stats[i][2], car_stats[i][3]);
        auto car2 = std::make_unique<Car>(car_stats[i][0], car_stats[i][1], car_stats[i][2], car_stats[i][3]);

        auto driver1 = std::make_unique<Driver>(driver1_name, driver1_exp, driver1_craft, 
            driver1_aware, driver1_pace, driver1_age, driver1_dry, driver1_inter, driver1_wet);
            
        auto driver2 = std::make_unique<Driver>(driver2_name, driver2_exp, driver2_craft, 
            driver2_aware, driver2_pace, driver2_age, driver2_dry, driver2_inter, driver2_wet);

            float avg_rating = static_cast<float>(car1->get_rating() + car2->get_rating()) / 2.0f;

       if (avg_rating > 85) {
            teams.push_back(std::make_unique<TopTeam>(team_name, std::move(car1), std::move(car2), 
                std::move(driver1), std::move(driver2), expected_position,
                dry_bonus, inter_bonus, wet_bonus, night_bonus));
        } else {
            teams.push_back(std::make_unique<Team>(team_name, std::move(car1), std::move(car2), 
                std::move(driver1), std::move(driver2), expected_position,
                dry_bonus, inter_bonus, wet_bonus, night_bonus));
        }
    }

    std::cout << "Select your team:\n";
    for (size_t i = 0; i < teams.size(); ++i) {
        std::cout << i + 1 << ". " << teams[i]->get_name() << "\n";
    }

    size_t choice;
    if (!(std::cin >> choice) || choice < 1 || choice > teams.size()) {
        throw InvalidTeamException("Invalid team selection: must be between 1 and " + std::to_string(teams.size()));
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