#include "GameManager.h"
#include "Menu.h"
#include "Exceptions.h"
#include "RegulationChanges.h"
#include "ConsoleView.h"

int main() {
    ConsoleView view;
    try {
        GameManager game_manager;
        if (!game_manager.initialize(view)) {
            return 1;
        }

        int num_seasons = view.askChoice("How many seasons would you like to play? (1-10): ", 1, 10);
        if (num_seasons < 1) {
            view.showMessage("Invalid number of seasons. Defaulting to 1 season.\n");
            num_seasons = 1;
        }

        for (int current_season = 1; current_season <= num_seasons; current_season++) {
            view.showMessage("\n=== Starting Season " + std::to_string(current_season) + " ===\n");
            if (current_season > 1 && (current_season - 1) % 3 == 0) {
                for (const auto& event : RegulationChanges::apply_regulation_changes(game_manager.get_teams())) {
                    view.showMessage(event);
                }
            }

            std::vector<Team*> team_ptr;
            team_ptr.reserve(game_manager.get_teams().size());
            for (const auto& team : game_manager.get_teams()) {
                team_ptr.push_back(team.get());
            }

            try {
                Season season(team_ptr, static_cast<int>(game_manager.get_circuits().size()));
                const Player player(game_manager.get_my_team());
                Menu::init(game_manager, const_cast<Player&>(player), season, view);
                Menu::getInstance().run();

                for (const auto& team : game_manager.get_teams()) {
                    for (const auto& event : team->convert_points_to_budget()) {
                        view.showMessage(event);
                    }

                    if (const auto d1 = team->get_driver_car(1).driver) {
                        d1->increase_age();
                    }
                    if (const auto d2 = team->get_driver_car(2).driver) {
                        d2->increase_age();
                    }
                    if (const auto r1 = team->get_reserve_driver(1)) {
                        r1->increase_age();
                    }
                    if (const auto r2 = team->get_reserve_driver(2)) {
                        r2->increase_age();
                    }

                    for (const auto& event : team->check_retirements()) {
                        view.showMessage(event);
                    }
                }

            } catch (const RaceWeekendException& e) {
                std::cerr << "Race Weekend Error: " << e.what() << "\n";
                return 1;
            } catch (const InvalidTeamException& e) {
                std::cerr << "Team Error: " << e.what() << "\n";
                return 1;
            }
        }

    } catch (const ConfigurationFileException& e) {
        std::cerr << "Configuration Error: " << e.what() << "\n";
        return 1;
    } catch (const Exceptions& e) {
        std::cerr << "General Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
