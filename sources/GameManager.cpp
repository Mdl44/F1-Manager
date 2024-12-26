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
    
        std::getline(team_file, team_name);
        team_file >> expected_position;
        team_file.ignore();

        std::string driver1_name;
        int driver1_exp, driver1_craft, driver1_aware, driver1_pace;
        int driver1_age;
        
        std::getline(team_file, driver1_name);
        team_file >> driver1_exp >> driver1_craft >> driver1_aware >> driver1_pace
                  >> driver1_age;
        team_file.ignore();

        std::string driver2_name;
        int driver2_exp, driver2_craft, driver2_aware, driver2_pace;
        int driver2_age;
        
        std::getline(team_file, driver2_name);
        team_file >> driver2_exp >> driver2_craft >> driver2_aware >> driver2_pace
                  >> driver2_age;
        team_file.ignore();

        auto car1 = std::make_unique<Car>(car_stats[i][0], car_stats[i][1], car_stats[i][2], car_stats[i][3]);
        auto car2 = std::make_unique<Car>(car_stats[i][0], car_stats[i][1], car_stats[i][2], car_stats[i][3]);

        auto driver1 = std::make_unique<Driver>(driver1_name, driver1_exp, driver1_craft, 
            driver1_aware, driver1_pace, driver1_age);
            
        auto driver2 = std::make_unique<Driver>(driver2_name, driver2_exp, driver2_craft, 
            driver2_aware, driver2_pace, driver2_age);

        std::string reserve1_name;
        int reserve1_exp, reserve1_craft, reserve1_aware, reserve1_pace;
        int reserve1_age;
        
        std::getline(team_file, reserve1_name);
        team_file >> reserve1_exp >> reserve1_craft >> reserve1_aware >> reserve1_pace
                  >> reserve1_age;
        team_file.ignore();

        std::string reserve2_name;
        int reserve2_exp, reserve2_craft, reserve2_aware, reserve2_pace;
        int reserve2_age;
        
        std::getline(team_file, reserve2_name);
        team_file >> reserve2_exp >> reserve2_craft >> reserve2_aware >> reserve2_pace
                  >> reserve2_age;
        team_file.ignore();

        auto reserve1 = std::make_unique<Driver>(reserve1_name, reserve1_exp, reserve1_craft,
            reserve1_aware, reserve1_pace, reserve1_age);
            
        auto reserve2 = std::make_unique<Driver>(reserve2_name, reserve2_exp, reserve2_craft,
            reserve2_aware, reserve2_pace, reserve2_age);


        float avg_rating = static_cast<float>(car1->get_performance().overall_rating + car2->get_performance().overall_rating) / 2.0f;
        if (avg_rating > 85) {
            teams.push_back(std::make_unique<TopTeam>(
                i,
                team_name, std::move(car1), std::move(car2),
                std::move(driver1), std::move(driver2),
                std::move(reserve1), std::move(reserve2),
                expected_position
            ));
        } else {
            teams.push_back(std::make_unique<Team>(
                i,
                team_name, std::move(car1), std::move(car2),
                std::move(driver1), std::move(driver2),
                std::move(reserve1), std::move(reserve2),
                expected_position
            ));
        }
    }

    std::cout << "Choose your option:\n";
    std::cout << "1. Select existing team\n";
    std::cout << "2. Create custom team\n";
    
    int option;
    std::cin >> option;
    std::cin.ignore();

    if (option == 1) {
        std::cout << "\nSelect your team:\n";
        for (size_t i = 0; i < teams.size(); ++i) {
            std::cout << i + 1 << ". " << teams[i]->get_name() << "\n";
        }

        size_t choice;
        if (!(std::cin >> choice) || choice < 1 || choice > teams.size()) {
            throw InvalidTeamException("Invalid team selection");
        }
        my_team = teams[choice - 1].get();
    }
    else if (option == 2) {
        constexpr float initial_budget = 50.0f;
        
        std::cout << "Enter your team name: ";
        std::string custom_team_name;
        std::getline(std::cin, custom_team_name);

        auto custom_car1 = std::make_unique<Car>(55, 55, 55, 55);
        auto custom_car2 = std::make_unique<Car>(55, 55, 55, 55);

        std::vector<std::unique_ptr<Driver>> available_drivers;
        std::ifstream pool_file("driver_pool.txt");
        if (!pool_file) {
            throw ConfigurationFileException("driver_pool.txt");
        }

        std::string driver_name;
        int exp, craft, aware, pace, age;
        while (std::getline(pool_file, driver_name)) {
            if (!(pool_file >> exp >> craft >> aware >> pace >> age)) {
            break;
            }
        pool_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        available_drivers.push_back(std::make_unique<Driver>(
            driver_name, exp, craft, aware, pace, age
        ));
    }

        std::vector<std::unique_ptr<Driver>> selected_drivers;
        float remaining_budget = initial_budget;

        std::cout << "\nSelect 4 drivers (2 main, 2 reserve) within budget of " << initial_budget << ":\n";
        
        for (int i = 1; i <= 4; i++) {
            std::cout << "\nSelecting " << (i <= 2 ? "main" : "reserve") << " driver " 
                     << (i <= 2 ? i : i-2) << "\n";
            std::cout << "Remaining budget: " << remaining_budget << "\n\n";
            
            for (size_t j = 0; j < available_drivers.size(); j++) {
                const auto& driver = available_drivers[j];
                float driver_value = driver->get_performance().market_value;
                if (driver_value <= remaining_budget) {
                    std::cout << j + 1 << ". " << driver->get_name() 
                             << " (Value: " << driver_value << ")\n";
                }
            }

            size_t choice;
            std::cin >> choice;
            if (choice < 1 || choice > available_drivers.size()) {
                throw InvalidDriverException("Invalid driver selection");
            }

            float selected_value = available_drivers[choice-1]->get_performance().market_value;
            if (selected_value > remaining_budget) {
                throw InvalidDriverException("Insufficient budget for selected driver");
            }

            remaining_budget -= selected_value;
            selected_drivers.push_back(std::move(available_drivers[choice-1]));
            available_drivers.erase(available_drivers.begin() + static_cast<std::ptrdiff_t>(choice - 1));
        }


        teams.push_back(std::make_unique<Team>(
            teams.size(),
            custom_team_name,
            std::move(custom_car1),
            std::move(custom_car2),
            std::move(selected_drivers[0]),
            std::move(selected_drivers[1]),
            std::move(selected_drivers[2]),
            std::move(selected_drivers[3]),
            9
        ));

        my_team = teams.back().get();
    }
    else {
        throw InvalidTeamException("Invalid option selection");
    }

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