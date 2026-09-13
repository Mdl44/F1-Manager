#include "Menu.h"
#include <sstream>
#include <cstdlib>
#include <stdexcept>

Menu::Menu(GameManager& manager, Player& player, Season& season, GameView& view)
    : manager(manager), player(player), season(season), view(view), my_team(manager.get_my_team()) {}

void Menu::displayMenu() const {
    view.showMessage(
        "\n=== F1 Season Menu ===\n"
        "1. View Team Data\n"
        "2. Apply Upgrades\n"
        "3. Continue to Next Race\n"
        "4. Driver Swap\n"
        "5. Exit Season\n"
        "6. Exit Game\n"
    );
}

Menu* Menu::instance = nullptr;

void Menu::init(GameManager& manager, Player& player, Season& season, GameView& view) {
    if (!instance) {
        instance = new Menu(manager, player, season, view);
    }
}

Menu& Menu::getInstance() {
    if (!instance) {
        throw std::runtime_error("Menu not initialized! Call Menu::init first.");
    }
    return *instance;
}

bool Menu::handleDriver_swap() const {
    std::ostringstream driverHeader;
    driverHeader << "\nSelect your driver to swap (1 or 2): \n"
                 << "1. " << my_team->get_driver_car(1).driver->get_name() << "\n"
                 << "2. " << my_team->get_driver_car(2).driver->get_name() << "\n";

    const int driverNum = view.askChoice(driverHeader.str(), 1, 2);
    if (driverNum < 1) {
        view.showMessage("Invalid driver selection!\n");
        return false;
    }

    const Driver* my_driver = my_team->get_driver_car(driverNum).driver;

    const int swapChoice = view.askChoice(
        "\nAvailable drivers for swap:\n"
        "1. Drivers from other teams\n"
        "2. Reserve drivers\n",
        1, 2);
    if (swapChoice < 1) {
        view.showMessage("Invalid choice!\n");
        return false;
    }

    if (swapChoice == 1) {
        std::ostringstream teamsList;
        teamsList << "\nAvailable teams for swap:\n";
        size_t displayedIndex = 1;
        std::vector<size_t> teamIndices;

        for (size_t i = 0; i < manager.get_teams().size(); ++i) {
            if (manager.get_teams()[i].get() != my_team) {
                teamsList << displayedIndex << ". " << manager.get_teams()[i]->get_name() << "\n";
                teamIndices.push_back(i);
                ++displayedIndex;
            }
        }
        teamsList << "\nSelect a team to swap with: ";

        const int teamChoice = view.askChoice(teamsList.str(), 1, static_cast<int>(teamIndices.size()));
        if (teamChoice < 1) {
            view.showMessage("Invalid team selection!\n");
            return false;
        }

        Team* selectedTeam = manager.get_teams()[teamIndices[teamChoice - 1]].get();

        std::ostringstream driverPrompt;
        driverPrompt << "\nSelect a driver from " << selectedTeam->get_name() << " to swap with:\n"
                     << "1. " << selectedTeam->get_driver_car(1).driver->get_name() << "\n"
                     << "2. " << selectedTeam->get_driver_car(2).driver->get_name() << "\n";

        const int targetDriverNum = view.askChoice(driverPrompt.str(), 1, 2);
        if (targetDriverNum < 1) {
            view.showMessage("Invalid driver selection!\n");
            return false;
        }

        const Driver* other_driver = selectedTeam->get_driver_car(targetDriverNum).driver;

        if (player.swap_try(view, my_driver, other_driver, *selectedTeam)) {
            view.showMessage("Swap successful!\n");
            return true;
        }

        view.showMessage("Swap failed - insufficient market value.\n");
        return false;

    } else {
        std::ostringstream reserveList;
        reserveList << "\nAvailable reserve drivers:\n";
        size_t displayedIndex = 1;
        std::vector<std::pair<Team*, int>> reserveIndices;

        for (const auto& team : manager.get_teams()) {
            if (const auto* reserve1 = team->get_reserve_driver(1)) {
                reserveList << displayedIndex << ". " << team->get_name() << " - "
                            << reserve1->get_name() << "\n";
                reserveIndices.emplace_back(team.get(), 1);
                ++displayedIndex;
            }
            if (const auto* reserve2 = team->get_reserve_driver(2)) {
                reserveList << displayedIndex << ". " << team->get_name() << " - "
                            << reserve2->get_name() << "\n";
                reserveIndices.emplace_back(team.get(), 2);
                ++displayedIndex;
            }
        }

        const int reserveChoice = view.askChoice(reserveList.str(), 1, static_cast<int>(reserveIndices.size()));
        if (reserveChoice < 1) {
            view.showMessage("Invalid reserve driver selection!\n");
            return false;
        }

        auto [selectedTeam, reserveIndex] = reserveIndices[reserveChoice - 1];
        const Driver* reserve_driver = selectedTeam->get_reserve_driver(reserveIndex);

        if (selectedTeam->swap_with_reserve(my_driver, reserve_driver)) {
            view.showMessage("Swap with reserve driver successful!\n");
            return true;
        }

        view.showMessage("Swap with reserve driver failed.\n");
        return false;
    }
}

bool Menu::handleChoice(size_t& current_race) const {
    const int action = view.askChoice("Enter your choice: ", 1, 6);
    if (action < 1) {
        view.showMessage("Invalid choice! Please try again.\n");
        return true;
    }

    switch (action) {
        case 1:
            player.show_data(view);
            break;
        case 2:
            player.upgrades(view);
            break;
        case 3: {
            const RaceOutcome outcome = season.race(*manager.get_circuits()[current_race]);
            view.showMessage(outcome.weekend_report);
            view.showMessage(outcome.fastest_lap_report);
            for (const auto& event : outcome.team_events) {
                view.showMessage(event);
            }
            if (!outcome.season_analysis.empty()) {
                view.showMessage(outcome.season_analysis);
            }
            view.showMessage(outcome.standings_report);
            ++current_race;
            break;
        }
        case 4:
            if (!handleDriver_swap()) {
                view.showMessage("Driver swap failed. Try again.\n");
            }
            break;
        case 5:
            view.showMessage("Exiting the season.\n");
            return false;
        case 6:
            view.showMessage("Exiting the game.\n");
            exit(0);
        default:
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

    view.showMessage("Season complete!\n");
    std::ostringstream oss;
    oss << season;
    view.showMessage(oss.str());
    view.askLine("Press any key to exit...");
}
