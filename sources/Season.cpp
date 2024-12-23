#include "Season.h"
#include <algorithm>
#include <iostream>
#include "Exceptions.h"
#include "WeatherConditionFactory.h"

Season::Season(const std::vector<Team*>& team_list, const int total_races)
    : teams(team_list), races(total_races) {
    if (team_list.empty()) {
        throw InvalidTeamException("Cannot create season: No teams provided");
    }
    if (total_races <= 0) {
        throw RaceWeekendException("Cannot create season: Invalid number of races");
    }
    
    for (const Team* team : teams) {
        if (!team) {
            throw InvalidTeamException("Cannot create season: Null team pointer");
        }
        if (const auto* d1 = team->get_driver_car(1).driver) {
            driver_points[d1->get_name()] = 0;
        }
        if (const auto* d2 = team->get_driver_car(2).driver) {
            driver_points[d2->get_name()] = 0;
        }
        team_points[team->get_name()] = 0;
    }
}  
Season::~Season() {
    std::cout << "Destroying season." << std::endl;
}

Season::Season(const Season& other)
    : teams(other.teams),
      driver_points(other.driver_points),
      team_points(other.team_points),
      races(other.races),
      current_race(other.current_race) {
    if (teams.empty()) {
        throw InvalidTeamException("Season must have teams");
    }
}

Season& Season::operator=(const Season& other) {
    if (this != &other) {
        teams = other.teams;
        driver_points = other.driver_points;
        team_points = other.team_points;
        races = other.races;
        current_race = other.current_race;
    }
    return *this;
}

int Season::calculate_combined_rating(const Team* team, const Driver* driver) {
    if (team->get_driver_car(1).driver == driver) {
        return (driver->get_performance().overall_rating + team->get_driver_car(1).car->get_rating()) / 2;
    }
    if (team->get_driver_car(2).driver == driver) {
        return (driver->get_performance().overall_rating + team->get_driver_car(2).car->get_rating()) / 2;
    }
    return 0;
}

void Season::race(RaceWeekend& weekend) {

    std::vector<std::pair<Driver*, int>> combined_ratings;
    weekend.set_teams(teams);
    
    weekend.set_quali_weather(WeatherConditionFactory::getWeather(
        weekend.night(), weekend.can_rain(),true));
        
    weekend.set_race_weather(WeatherConditionFactory::getWeather(
        weekend.night(), weekend.can_rain(), false));

    for (const Team* team : teams) {
        if (auto* d1 = team->get_driver_car(1).driver) {
            combined_ratings.emplace_back(d1, calculate_combined_rating(team, d1));
        }
        if (auto* d2 = team->get_driver_car(2).driver) {
            combined_ratings.emplace_back(d2, calculate_combined_rating(team, d2));
        }
    }

    weekend.quali(combined_ratings);
    const auto results = weekend.race();
    std::cout << weekend;
    standings(results);
    std::cout << *this;
    current_race++;
}

void Season::standings(const std::vector<std::pair<Driver*, long long>>& race_results) {
    const int points[] = {25, 18, 15, 12, 10, 8, 6, 4, 2, 1};
    for (size_t i = 0; i < race_results.size() && i < 10; i++) {
        const Driver* driver = race_results[i].first;
        driver_points[driver->get_name()] += points[i];

        for (const Team* team : teams) {
            if (team->get_driver_car(1).driver == driver || team->get_driver_car(2).driver == driver) {
                team_points[team->get_name()] += points[i];
                break;
            }
        }
    }
    update_team_performance();
}

void Season::update_team_performance() {
    std::vector<std::pair<std::string, int>> sorted_standings(team_points.begin(), team_points.end());
    std::ranges::sort(sorted_standings, [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    for (size_t pos = 0; pos < sorted_standings.size(); pos++) {
        const auto& [team_name, _] = sorted_standings[pos];
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

void Season::printStandings(std::ostream& os, const std::vector<std::pair<std::string, int>>& standings, const std::string& title, const int lungime) {
    os << title << "\n";
    os << std::string(60, '-') << "\n";

    int pos = 1;
    for (const auto& [name, points] : standings) {
        std::string pos_str = (pos < 10 ? " " : "") + std::to_string(pos) + ".";
        std::string formatted_name = name;
        if (static_cast<int>(formatted_name.length()) < lungime) {
            formatted_name += std::string(lungime - formatted_name.length(), ' ');
        }
        os << pos_str << " " << formatted_name << points << " pts\n";
        pos++;
    }
}

std::ostream& operator<<(std::ostream& os, const Season& season) {
    if (season.current_race < season.races)
        os << "\nSeason Standings after Race " << season.current_race << " of " << season.races << "\n\n";

    std::vector<std::pair<std::string, int>> driver_standings(
        season.driver_points.begin(),
        season.driver_points.end()
    );
    std::ranges::sort(driver_standings, [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    Season::printStandings(os, driver_standings, "Driver Championship", 25);

    std::vector<std::pair<std::string, int>> team_standings(
        season.team_points.begin(),
        season.team_points.end()
    );
    std::ranges::sort(team_standings, [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    Season::printStandings(os, team_standings, "\nConstructor Championship", 35);

    if (season.current_race == season.races) {
        os << "\nFINAL RESULTS\n";
        os << std::string(60, '*') << "\n";
        if (!driver_standings.empty()) {
            const auto& champion = driver_standings[0];
            os << "World Drivers' Champion: " << champion.first 
               << " with " << champion.second << " points\n";
        }
        if (!team_standings.empty()) {
            const auto& champion = team_standings[0];
            os << "Constructors' Champion: " << champion.first
               << " with " << champion.second << " points\n";
        }
        os << std::string(60, '*') << "\n";
    }
    
    return os;
}