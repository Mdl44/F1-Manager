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
    auto pad_string = [](const std::string& str, size_t width) {
        if (str.length() >= width) return str;
        return str + std::string(width - str.length(), ' ');
    };

    std::cout << "\nDriver Championship\n";
    std::cout << std::string(60, '-') << "\n";

    std::vector<std::pair<std::string, int>> drivers_standings(driver_points.begin(), driver_points.end());
    std::ranges::sort(drivers_standings, [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    int pos = 1;
    for (const auto& [name, points] : drivers_standings) {
        std::string pos_str = (pos < 10 ? " " : "") + std::to_string(pos) + ".";
        std::cout << pos_str << " "
                  << pad_string(name, 25)
                  << points << " pts\n";
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
        std::cout << pos_str << " "
                  << pad_string(name, 35)
                  << points << " pts\n";
        pos++;
    }
    std::cout << std::string(60, '-') << "\n\n";
}

std::vector<std::pair<std::string, int>> Season::get_driver_standings() const {
    std::vector<std::pair<std::string, int>> standings(driver_points.begin(), driver_points.end());
    std::ranges::sort(standings, [](const auto& a, const auto& b) {
        return a.second > b.second;
    });
    return standings;
}

std::vector<std::pair<std::string, int>> Season::get_team_standings() const {
    std::vector<std::pair<std::string, int>> standings(team_points.begin(), team_points.end());
    std::ranges::sort(standings, [](const auto& a, const auto& b) {
        return a.second > b.second;
    });
    return standings;
}