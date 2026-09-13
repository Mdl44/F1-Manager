#include "GameManager.h"
#include <fstream>
#include <sstream>
#include "TopTeam.h"
#include "Exceptions.h"
#include "GameRules.h"

namespace {
    // A reserve name of "NONE" means the slot has no confirmed driver.
    constexpr auto NO_RESERVE = "NONE";

    std::unique_ptr<Driver> read_driver(std::ifstream& file, const bool optional) {
        std::string name;
        std::getline(file, name);

        if (optional && name == NO_RESERVE) {
            return nullptr;
        }

        int exp, craft, aware, pace, age;
        file >> exp >> craft >> aware >> pace >> age;
        file.ignore();

        return std::make_unique<Driver>(name, exp, craft, aware, pace, age);
    }

    void load_circuits(const std::string& path, std::vector<std::unique_ptr<RaceWeekend>>& target) {
        std::ifstream file(path);
        if (!file) {
            throw ConfigurationFileException(path);
        }

        std::string name;
        int laps, reference_time;
        bool rain, night_race;
        while (std::getline(file, name)) {
            if (!(file >> reference_time >> laps >> night_race >> rain)) {
                throw ConfigurationFileException(path + ": incomplete data for circuit '" + name + "'");
            }
            file.ignore();
            target.emplace_back(std::make_unique<RaceWeekend>(name, laps, reference_time, rain, night_race));
        }
    }
}

GameManager::GameManager() : my_team(nullptr) {}

bool GameManager::initialize(GameView& view) {
    std::vector<std::vector<int>> car_stats;
    std::ifstream car_file("car_data.txt");
    if (!car_file) {
        throw ConfigurationFileException("car_data.txt");
    }

    int aero, power, durability, chassis;
    while (car_file >> aero >> power >> durability >> chassis) {
        car_stats.push_back({aero, power, durability, chassis});
    }

    load_circuits("circuits.txt", circuits);

    std::ifstream team_file("teams.txt");
    if (!team_file) {
        throw ConfigurationFileException("teams.txt");
    }

    int num_teams;
    team_file >> num_teams;
    team_file.ignore();

    if (static_cast<size_t>(num_teams) > car_stats.size()) {
        throw ConfigurationFileException(
            "car_data.txt: only " + std::to_string(car_stats.size()) +
            " car entries for " + std::to_string(num_teams) + " teams declared in teams.txt");
    }

    for (size_t i = 0; i < static_cast<size_t>(num_teams) && i < car_stats.size(); ++i) {
        std::string team_name;
        int expected_position;
    
        std::getline(team_file, team_name);
        team_file >> expected_position;
        team_file.ignore();

        auto driver1 = read_driver(team_file, false);
        auto driver2 = read_driver(team_file, false);
        auto reserve1 = read_driver(team_file, true);
        auto reserve2 = read_driver(team_file, true);

        auto car1 = std::make_unique<Car>(car_stats[i][0], car_stats[i][1], car_stats[i][2], car_stats[i][3]);
        auto car2 = std::make_unique<Car>(car_stats[i][0], car_stats[i][1], car_stats[i][2], car_stats[i][3]);

        float avg_rating = static_cast<float>(car1->get_performance().overall_rating + car2->get_performance().overall_rating) / 2.0f;
        if (avg_rating > GameRules::Team::TOP_TEAM_RATING_THRESHOLD) {
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

    offer_legacy_circuits(view);

    const int option = view.askChoice(
        "Choose your option:\n"
        "1. Select existing team\n"
        "2. Create custom team\n",
        1, 2);

    if (option == 1) {
        std::ostringstream team_list;
        team_list << "\nSelect your team:\n";
        for (size_t i = 0; i < teams.size(); ++i) {
            team_list << i + 1 << ". " << teams[i]->get_name() << "\n";
        }

        const int choice = view.askChoice(team_list.str(), 1, static_cast<int>(teams.size()));
        if (choice < 1) {
            throw InvalidTeamException("Invalid team selection");
        }
        my_team = teams[choice - 1].get();
    }
    else if (option == 2) {
        constexpr float initial_budget = GameRules::CustomTeam::INITIAL_BUDGET;

        const std::string custom_team_name = view.askLine("Enter your team name: ");

        constexpr int base_stat = GameRules::CustomTeam::BASE_CAR_STAT;
        auto custom_car1 = std::make_unique<Car>(base_stat, base_stat, base_stat, base_stat);
        auto custom_car2 = std::make_unique<Car>(base_stat, base_stat, base_stat, base_stat);

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

        std::ostringstream budget_intro;
        budget_intro << "\nSelect 4 drivers (2 main, 2 reserve) within budget of " << initial_budget << ":\n";
        view.showMessage(budget_intro.str());

        for (int i = 1; i <= 4; i++) {
            std::ostringstream driver_list;
            driver_list << "\nSelecting " << (i <= 2 ? "main" : "reserve") << " driver "
                        << (i <= 2 ? i : i - 2) << "\n"
                        << "Remaining budget: " << remaining_budget << "\n\n";

            for (size_t j = 0; j < available_drivers.size(); j++) {
                const auto& driver = available_drivers[j];
                float driver_value = driver->get_performance().market_value;
                if (driver_value <= remaining_budget) {
                    driver_list << j + 1 << ". " << driver->get_name()
                                << " (Value: " << driver_value << ")\n";
                }
            }

            const int choice = view.askChoice(driver_list.str(), 1, static_cast<int>(available_drivers.size()));
            if (choice < 1) {
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
            static_cast<int>(teams.size()) + 1 // a new custom team starts at the back of the grid
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

void GameManager::offer_legacy_circuits(GameView& view) {
    std::vector<std::unique_ptr<RaceWeekend>> legacy;
    load_circuits("legacy_circuits.txt", legacy);

    view.showMessage(
        "\nA few real circuits were dropped from the 2026 calendar (cancelled "
        "or not renewed). You can add any of them back as bonus rounds at the "
        "end of your season.\n");

    for (auto& circuit : legacy) {
        const int choice = view.askChoice(
            "Add " + circuit->get_name() + " to your calendar? (1 = Yes, 2 = No): ", 1, 2);
        if (choice == 1) {
            circuits.push_back(std::move(circuit));
        }
    }
}

std::vector<std::unique_ptr<RaceWeekend>>& GameManager::get_circuits() {
    return circuits;
}