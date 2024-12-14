#include "Menu.h"
#include <iostream>

Menu::Menu(GameManager& manager, Player& player, Season& season)
    : manager(manager), player(player), season(season), my_team(manager.get_my_team()) {}

void Menu::displayMenu() {
    std::cout << "\n=== F1 Season Menu ===\n";
    std::cout << "1. View Team Data\n";
    std::cout << "2. Apply Upgrades\n";
    std::cout << "3. Continue to Next Race\n";
    std::cout << "4. Driver Swap\n";
    std::cout << "5. Exit Season\n";
    std::cout << "Enter your choice: ";
}

bool Menu::isValidNumber(int& number) {
    if (!(std::cin >> number)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return false;
    }
    return true;
}

bool Menu::handleDriver_swap() const {
    std::cout << "\nSelect your driver to swap (1 or 2): \n";
    std::cout << "1. " << my_team->get_driver_car(1).driver->get_name() << "\n";
    std::cout << "2. " << my_team->get_driver_car(2).driver->get_name() << "\n";

    int driverNum;
    if (!isValidNumber(driverNum) || (driverNum != 1 && driverNum != 2)) {
        std::cout << "Invalid driver selection!\n";
        return false;
    }

    const Driver* my_driver = my_team->get_driver_car(driverNum).driver;

    std::cout << "\nAvailable teams for swap:\n";
    size_t displayedIndex = 1;
    std::vector<size_t> teamIndices;

    for (size_t i = 0; i < manager.get_teams().size(); ++i) {
        if (manager.get_teams()[i].get() != my_team) {
            std::cout << displayedIndex << ". " << manager.get_teams()[i]->get_name() << "\n";
            teamIndices.push_back(i);
            ++displayedIndex;
        }
    }

    std::cout << "\nSelect a team to swap with: ";
    int teamChoice;
    if (!isValidNumber(teamChoice) || teamChoice < 1 ||
        teamChoice > static_cast<int>(teamIndices.size())) {
        std::cout << "Invalid team selection!\n";
        return false;
    }

    Team* selectedTeam = manager.get_teams()[teamIndices[teamChoice - 1]].get();

    std::cout << "\nSelect a driver from " << selectedTeam->get_name() << " to swap with:\n";
    std::cout << "1. " << selectedTeam->get_driver_car(1).driver->get_name() << "\n";
    std::cout << "2. " << selectedTeam->get_driver_car(2).driver->get_name() << "\n";

    int targetDriverNum;
    if (!isValidNumber(targetDriverNum) || (targetDriverNum != 1 && targetDriverNum != 2)) {
        std::cout << "Invalid driver selection!\n";
        return false;
    }

    const Driver* other_driver = selectedTeam->get_driver_car(targetDriverNum).driver;

    if (player.swap_try(my_driver, other_driver, *selectedTeam)) {
        std::cout << "Swap successful!\n";
        return true;
    }

    std::cout << "Swap failed - insufficient market value.\n";
    return false;
}

bool Menu::handleChoice(size_t& current_race) const {
    int action;
    if (!isValidNumber(action)) {
        std::cout << "Invalid input! Please enter a number.\n";
        return true;
    }

    switch (action) {
        case 1:
            player.show_data();
            break;
        case 2:
            player.upgrades();
            break;
        case 3:
            season.race(*manager.get_circuits()[current_race]);
            ++current_race;
            break;
        case 4:
            if (!handleDriver_swap()) {
                std::cout << "Driver swap failed. Try again.\n";
            }
        break;
        case 5:
            std::cout << "Exiting the season.\n";
            return false;
        default:
            std::cout << "Invalid choice! Please try again.\n";
            break;
    }
    return true;
}

void Menu::run() const {
    size_t currentRace = 0;
    while (currentRace < manager.get_circuits().size()) {
        displayMenu();
        if (!handleChoice(currentRace)) {
            return;
        }
    }

    std::cout << "Season complete!\n";
    std::cout << season;
    std::cout << "Press any key to exit...";
    std::cin.ignore(10000, '\n');
    std::cin.get();
}
