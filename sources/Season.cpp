#include "Season.h"
#include <algorithm>

Season::Season(const std::vector<Team*>& teamList, int races)
    : teams(teamList), races(races) {
    for (const auto& team : teams) {
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
    std::vector<Driver*> drivers_team = drivers;
    weekend.quali(drivers_team);
    weekend.display_quali();
    auto results = weekend.race();
    weekend.display_race();
    standings(results);
    display_standings();
    current_race++;
}

void Season::standings(const std::vector<std::pair<Driver*, long long>>& results) {
    const int points[] = {25, 18, 15, 12, 10, 8, 6, 4, 2, 1};
    for (size_t i = 0; i < results.size() && i < 10; i++) {
        Driver* driver = results[i].first;
        driver_points[driver->get_name()] += points[i];
        if (Team* team = driver->get_team()) {
            std::cout << driver->get_name() << " scores " << points[i] << " points for " << team->get_name() << "\n";
            team_points[team->get_name()] += points[i];
        }
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

/*std::vector<std::pair<std::string, int>> Season::get_driver_standings() const {
    std::vector<std::pair<std::string, int>> driver_standings(driver_points.begin(), driver_points.end());
    std::ranges::sort(driver_standings, [](const auto& a, const auto& b) {
        return a.second > b.second;
    });
    return driver_standings;
}
*/

/*std::vector<std::pair<std::string, int>> Season::get_team_standings() const {
    std::vector<std::pair<std::string, int>> team_standings(team_points.begin(), team_points.end());
    std::ranges::sort(team_standings, [](const auto& a, const auto& b) {
        return a.second > b.second;
    });
    return team_standings;
}
*/
