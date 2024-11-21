#include "Season.h"
#include <algorithm>

Season::Season(const std::vector<Team*>& team_list, const int total_races)
    : teams(team_list), races(total_races) {
    for (const Team* team : teams) {
        if (auto d1 = team->get_driver1()) {
            drivers.push_back(d1);
            driver_points[d1->get_name()] = 0;
        }
        if (auto d2 = team->get_driver2()) {
            drivers.push_back(d2);
            driver_points[d2->get_name()] = 0;
        }
        team_points[team->get_name()] = 0;
    }
}

void Season::race(RaceWeekend& weekend) {
    const std::vector<Driver*> drivers_team = drivers;
    weekend.quali(drivers_team);
    weekend.display_quali();
    const auto results = weekend.race();
    weekend.display_race();
    standings(results);
    display_standings();
    current_race++;
}

void Season::standings(const std::vector<std::pair<Driver*, long long>>& race_results) {
    const int points[] = {25, 18, 15, 12, 10, 8, 6, 4, 2, 1};
    for (size_t i = 0; i < race_results.size() && i < 10; i++) {
        Driver* driver = race_results[i].first;
        driver_points[driver->get_name()] += points[i];
        if (const Team* team = driver->get_team()) {
            team_points[team->get_name()] += points[i];
        }
    }

    std::vector<std::pair<std::string, int>> team_standings(team_points.begin(), team_points.end());
    std::ranges::sort(team_standings, [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    int position = 1;
    for (const auto& team_standing : team_standings) {
        const std::string& team_name = team_standing.first;
        for (Team* team : teams) {
            if (team->get_name() == team_name) {
                team->update_performance_points(position);

                if (team->get_downgrade_points() > 0) {
                    team->apply_downgrade();
                }

                if (team->is_player_controlled()) {
                    apply_player_team_upgrades(team);
                } else if (team->get_upgrade_points() >= 5) {
                    apply_ai_team_upgrades(team);
                }
                break;
            }
        }
        position++;
    }
}

void Season::apply_ai_team_upgrades(Team* team) {
    if (team->get_upgrade_points() >= 5) {
        team->apply_upgrade_for_ai_team();
        std::cout << "AI Team Upgrade Applied\n";
    }
}

void Season::apply_player_team_upgrades(const Team* team) {
    int upgrades_to_apply = team->get_upgrade_points();
    if (upgrades_to_apply > 0) {
        std::cout << "Player Team Upgrade Available! Upgrades: " << upgrades_to_apply << "\n";
    }
}

void Season::display_standings() {
    std::cout << "\nDriver Championship\n";

    std::vector<std::pair<std::string, int>> drivers_standings(driver_points.begin(), driver_points.end());
    std::ranges::sort(drivers_standings, [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    int pos = 1;
    for (const auto& [name, points] : drivers_standings) {
        std::string pos_str = (pos < 10 ? " " : "") + std::to_string(pos) + ".";
        std::string formatted_name = name;
        while (formatted_name.length() < 25) {
            formatted_name += " ";
        }
        std::cout << pos_str << " " << formatted_name << points << " pts\n";
        pos++;
    }

    std::cout << "\nConstructor Championship\n";

    std::vector<std::pair<std::string, int>> team_standings(team_points.begin(), team_points.end());
    std::ranges::sort(team_standings, [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    pos = 1;
    for (const auto& [name, points] : team_standings) {
        std::string pos_str = (pos < 10 ? " " : "") + std::to_string(pos) + ".";
        std::string formatted_name = name;
        while (formatted_name.length() < 35) {
            formatted_name += " ";
        }
        std::cout << pos_str << " " << formatted_name << points << " pts\n";
        pos++;
    }
    std::cout << std::string(60, '-') << "\n";

    if (current_race == races) {
        std::cout << "\nFINAL CHAMPIONS\n";
        std::cout << std::string(60, '*') << "\n";
        if (!drivers_standings.empty()) {
            const auto& champion_driver = drivers_standings[0];
            std::cout << "World Drivers' Champions : " << champion_driver.first
                     << " with " << champion_driver.second << " points\n";
        }
        if (!team_standings.empty()) {
            const auto& champion_team = team_standings[0];
            std::cout << "Constructors Champion: " << champion_team.first
                     << " with " << champion_team.second << " points\n";
        }
        std::cout << std::string(60, '*') << "\n";
    }
    std::cout << "\n";
}
