#include "GameManager.h"
#include "Menu.h"

int main() {
    GameManager manager;
    if (!manager.initialize()) {
        return 1;
    }

    Team* my_team = manager.get_my_team();
    const Player player(my_team);

    std::vector<Team*> team_ptr;
    team_ptr.reserve(manager.get_teams().size());
    for (const auto& team : manager.get_teams()) {
        team_ptr.push_back(team.get());
    }
    Season season(team_ptr);

    const Menu menu(manager, const_cast<Player&>(player), season);
    menu.run();

    return 0;
}