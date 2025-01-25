#include "GameManager.h"
#include "Menu.h"
#include "Exceptions.h"
#include "RegulationChanges.h"

int main() {
    try {
        GameManager game_manager;
        if (!game_manager.initialize()) {
            return 1;
        }

        std::cout << "How many seasons would you like to play? (1-10): ";
        int num_seasons;
        std::cin >> num_seasons;
        
        if (num_seasons < 1 || num_seasons > 10) {
            std::cout << "Invalid number of seasons. Defaulting to 1 season.\n";
            num_seasons = 1;
        }

        for (int current_season = 1; current_season <= num_seasons; current_season++) {
            std::cout << "\n=== Starting Season " << current_season << " ===\n";
            if (current_season > 1 && (current_season - 1) % 3 == 0) {
                RegulationChanges::apply_regulation_changes(game_manager.get_teams());
            }
    
            std::vector<Team*> team_ptr;
            team_ptr.reserve(game_manager.get_teams().size());
            for (const auto& team : game_manager.get_teams()) {
                team_ptr.push_back(team.get());
            }

            try {
                Season season(team_ptr);
                const Player player(game_manager.get_my_team());
                Menu::init(game_manager, const_cast<Player&>(player), season);
                Menu::getInstance().run();

                for (const auto& team : game_manager.get_teams()) {
                    team->convert_points_to_budget();
                    
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

                    team->check_retirements();
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