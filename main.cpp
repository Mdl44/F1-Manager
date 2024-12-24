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
                const Menu menu(game_manager, const_cast<Player&>(player), season);
                menu.run();

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