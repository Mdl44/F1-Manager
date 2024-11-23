#include <fstream>
#include <memory>
#include "Player.h"
#include "Season.h"

void displayMenu() {
    std::cout << "\n=== F1 Season Menu ===\n";
    std::cout << "1. View Team Data\n";
    std::cout << "2. Apply Upgrades\n";
    std::cout << "3. Continue to Next Race\n";
    std::cout << "4. Driver Swap\n";
    std::cout << "5. Exit Season\n";
    std::cout << "Enter your choice: ";
}
bool isValidNumber(int &num) {
    if (!(std::cin >> num)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return false;
    }
    return true;
}

int main() {
    std::ifstream input_file("tastatura.txt");
    if (!input_file) {
        return 1;
    }
    std::vector<std::vector<int>> car_stats;
    std::ifstream car_file("date_masini.txt");
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

    std::cout << "Select your team:\n";
    for (size_t i = 0; i < teams.size(); ++i) {
        std::cout << i + 1 << ". " << teams[i]->get_name() << "\n";
    }
    size_t choice;
    std::cin >> choice;
    if (choice < 1 || choice > teams.size()) {
        std::cerr << "Invalid team selection!\n";
        return 1;
    }

    Team* userTeam = teams[choice - 1].get();
    Player player(userTeam);

    std::vector<Team*> team_ptr;
    team_ptr.reserve(teams.size());
    for (const auto& team : teams) {
        team_ptr.push_back(team.get());
    }
    Season season(team_ptr);

    size_t currentRace = 0;
    while (currentRace < circuits.size()) {
        displayMenu();
        int action;
        if (!isValidNumber(action)) {
            std::cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        switch (action) {
            case 1:
                player.show_data();
            break;

            case 2:
                player.upgrades();
            break;

            case 3:
                std::cout << "\nSimulating Race " << (currentRace + 1) << " - " << circuits[currentRace].get_name() << "...\n";
            season.race(circuits[currentRace]);
            ++currentRace;
            break;

            case 4: {
                std::cout << "\nSelect your driver to swap (1 or 2): \n";
                std::cout << "1. " << userTeam->get_driver1()->get_name() << "\n";
                std::cout << "2. " << userTeam->get_driver2()->get_name() << "\n";
                int driverNum;
                std::cin >> driverNum;

                if (driverNum != 1 && driverNum != 2) {
                    std::cout << "Invalid driver selection!\n";
                    break;
                }

                Driver* myDriver = (driverNum == 1) ? userTeam->get_driver1() : userTeam->get_driver2();

                std::cout << "\nAvailable teams for swap:\n";
                size_t displayedIndex = 1;
                std::vector<size_t> teamIndices;
                for (size_t i = 0; i < teams.size(); ++i) {
                    if (teams[i].get() != userTeam) {
                        std::cout << displayedIndex << ". " << teams[i]->get_name() << "\n";
                        teamIndices.push_back(i);
                        ++displayedIndex;
                    }
                }

                std::cout << "\nSelect a team to swap with: ";
                int teamChoice;
                std::cin >> teamChoice;

                if (teamChoice < 1 || teamChoice > static_cast<int>(teamIndices.size())) {
                    std::cout << "Invalid team selection!\n";
                    break;
                }

                Team* selectedTeam = teams[teamIndices[teamChoice - 1]].get();

                std::cout << "\nSelect a driver from " << selectedTeam->get_name() << " to swap with:\n";
                std::cout << "1. " << selectedTeam->get_driver1()->get_name() << "\n";
                std::cout << "2. " << selectedTeam->get_driver2()->get_name() << "\n";
                int targetDriverNum;
                if (!isValidNumber(targetDriverNum)) {
                    std::cout << "Invalid input! Please enter 1 or 2.\n";
                    break;
                }

                if (targetDriverNum != 1 && targetDriverNum != 2) {
                    std::cout << "Invalid driver selection!\n";
                    break;
                }

                Driver* otherDriver = (targetDriverNum == 1) ? selectedTeam->get_driver1() : selectedTeam->get_driver2();

                if (player.swap_try(myDriver, otherDriver, *selectedTeam)) {
                    std::cout << "Swap successful!\n";
                } else {
                    std::cout << "Swap failed - insufficient market value or invalid selection.\n";
                }
                break;
            }


            case 5:
                std::cout << "Exiting the season.\n";
            return 0;

            default:
                std::cout << "Invalid choice! Please try again.\n";
            break;
        }
    }
    std::cout << "Season complete!\n";
    season.display_standings();
    std::cout << "Press any key to exit...";
    std::cin.ignore(10000, '\n');
    std::cin.get();
    return 0;
}