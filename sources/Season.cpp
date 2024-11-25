#include "Season.h"
#include <algorithm>
#include <iostream>

Season::Season(const std::vector<Team*>& team_list, const int total_races)
    : teams(team_list), races(total_races) {
    for (const Team* team : teams) {
        if (auto* d1 = team->get_driver1()) {
            driver_points[d1->get_name()] = 0;
        }
        if (auto* d2 = team->get_driver2()) {
            driver_points[d2->get_name()] = 0;
        }
        team_points[team->get_name()] = 0;
    }
}

int Season::calculate_combined_rating(const Team* team, const Driver* driver) {
    if (team->get_driver1() == driver) {
        return (driver->get_rating() + team->get_car1()->get_rating()) / 2;
    }
    if (team->get_driver2() == driver) {
        return (driver->get_rating() + team->get_car2()->get_rating()) / 2;
    }
    return 0;
}

void Season::race(RaceWeekend& weekend) {
    std::vector<std::pair<Driver*, int>> combined_ratings;
    for (Team* team : teams) {
        if (auto* d1 = team->get_driver1()) {
            combined_ratings.emplace_back(d1, calculate_combined_rating(team, d1));
        }
        if (auto* d2 = team->get_driver2()) {
            combined_ratings.emplace_back(d2, calculate_combined_rating(team, d2));
        }
    }
    
    weekend.quali(combined_ratings);
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
        
        for (Team* team : teams) {
            if (team->get_driver1() == driver || team->get_driver2() == driver) {
                team_points[team->get_name()] += points[i];
                break;
            }
        }
    }
    update_team_performance();
}

void Season::update_team_performance() {
    std::vector<std::pair<std::string, int>> standings(team_points.begin(), team_points.end());
    std::ranges::sort(standings, [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    for (size_t pos = 0; pos < standings.size(); pos++) {
        const auto& [team_name, _] = standings[pos];
        for (Team* team : teams) {
            if (team->get_name() == team_name) {
                team->update_performance_points(static_cast<int>(pos + 1));
                
                if (team->get_downgrade_points() > 0) {
                    team->apply_downgrade();
                }
                
                if (team->is_player_controlled()) {
                    if (team->get_upgrade_points() > 0) {
                        std::cout << "Player Team has " << team->get_upgrade_points() << " upgrade points available.\n";
                    }
                } else if (team->get_upgrade_points() >= 5) {
                    team->apply_upgrade_for_ai_team();
                    std::cout << "AI Team " << team->get_name() << " applied upgrades.\n";
                }
                break;
            }
        }
    }
}

void Season::display_standings() {
    std::cout << "\nDriver Championship\n";
    std::cout << std::string(60, '-') << "\n";

    std::vector<std::pair<std::string, int>> driver_standings(driver_points.begin(), driver_points.end());
    std::ranges::sort(driver_standings, [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    int pos = 1;
    for (const auto& [name, points] : driver_standings) {
        std::string pos_str = (pos < 10 ? " " : "") + std::to_string(pos) + ".";
        std::string formatted_name = name;
        if (formatted_name.length() < 25) {
            formatted_name += std::string(25 - formatted_name.length(), ' ');
        }
        std::cout << pos_str << " " << formatted_name << points << " pts\n";
        pos++;
    }

    std::cout << "\nConstructor Championship\n";
    std::cout << std::string(60, '-') << "\n";

    std::vector<std::pair<std::string, int>> team_standings(team_points.begin(), team_points.end());
    std::ranges::sort(team_standings, [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    pos = 1;
    for (const auto& [name, points] : team_standings) {
        std::string pos_str = (pos < 10 ? " " : "") + std::to_string(pos) + ".";
        std::string formatted_name = name;
        if (formatted_name.length() < 35) {
            formatted_name += std::string(35 - formatted_name.length(), ' ');
        }
        std::cout << pos_str << " " << formatted_name << points << " pts\n";
        pos++;
    }

    if (current_race == races) {
        std::cout << "\nFINAL RESULTS\n";
        std::cout << std::string(60, '*') << "\n";
        if (!driver_standings.empty()) {
            const auto& champion = driver_standings[0];
            std::cout << "World Drivers' Champion: " << champion.first 
                     << " with " << champion.second << " points\n";
        }
        if (!team_standings.empty()) {
            const auto& champion = team_standings[0];
            std::cout << "Constructors' Champion: " << champion.first
                     << " with " << champion.second << " points\n";
        }
        std::cout << std::string(60, '*') << "\n";
    }
    std::cout << "\n";
}