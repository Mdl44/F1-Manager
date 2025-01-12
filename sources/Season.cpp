#include "Season.h"
#include <algorithm>
#include <iostream>
#include "Exceptions.h"
#include "WeatherConditionFactory.h"
#include "GameState.h"

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
        return (driver->get_performance().overall_rating + team->get_driver_car(1).car->get_performance().overall_rating) / 2;
    }
    if (team->get_driver_car(2).driver == driver) {
        return (driver->get_performance().overall_rating + team->get_driver_car(2).car->get_performance().overall_rating) / 2;
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
        if (const auto* car1 = team->get_driver_car(1).car) {
            car_ratings.add_value(team->get_name(), car1->get_performance().overall_rating);
        }
        if (const auto* car2 = team->get_driver_car(2).car) {
            car_ratings.add_value(team->get_name(), car2->get_performance().overall_rating);
        }

        if (auto* d1 = team->get_driver_car(1).driver) {
            driver_ratings.add_value(d1->get_name(), d1->get_performance().overall_rating);
            combined_ratings.emplace_back(d1, calculate_combined_rating(team, d1));
        }
        if (auto* d2 = team->get_driver_car(2).driver) {
            driver_ratings.add_value(d2->get_name(), d2->get_performance().overall_rating);
            combined_ratings.emplace_back(d2, calculate_combined_rating(team, d2));
        }
    }

    weekend.quali(combined_ratings);
    const auto& quali_results = weekend.get_quali_results();
    if (!quali_results.empty()) {
        quali_performances.add_value(quali_results[0].first->get_name(), 
                                   quali_results[0].second, true);
    }
    const auto results = weekend.race();
    if (!results.empty()) {
        race_performances.add_value(results[0].first->get_name(), 
                                  results[0].second, true);
    }
    std::cout << weekend;

    const auto race_laps = weekend.get_lap_times();
    auto [fastest_driver, fastest_time, lap_number] = find_fastest_lap<Driver*>(race_laps);
    
    const int minutes = static_cast<int>((fastest_time % (1000 * 60 * 60)) / (1000 * 60));
    const int seconds = static_cast<int>((fastest_time % (1000 * 60)) / 1000);
    const int milliseconds = static_cast<int>(fastest_time % 1000);
    
    std::cout << "\nFastest Lap:\n";
    std::cout << fastest_driver->get_name() << " - " 
              << minutes << ":"
              << (seconds < 10 ? "0" : "") << seconds << "."
              << (milliseconds < 100 ? "0" : "")
              << (milliseconds < 10 ? "0" : "")
              << milliseconds
              << " (Lap " << lap_number << ")\n";

    standings(results);
    if (current_race == races) {
        recordSeasonChampions();
        std::cout << "\nSEASON PERFORMANCE ANALYSIS\n";
        std::cout << std::string(60, '-') << "\n";
        
        auto best_cars = car_ratings.get_all_highest();
        std::cout << "Best Car" << (best_cars.size() > 1 ? "s" : "") << ":\n";
        for (const auto& [car, rating] : best_cars) {
            std::cout << "  " << car << " (Rating: " << rating << ")\n";
        }
        
        auto best_drivers = driver_ratings.get_all_highest();
        std::cout << "\nTop Driver Rating" << (best_drivers.size() > 1 ? "s" : "") << ":\n";
        for (const auto& [driver, rating] : best_drivers) {
            std::cout << "  " << driver << " (Rating: " << rating << ")\n";
        }
        
        auto pole_leaders = quali_performances.get_all_most_wins();
        std::cout << "\nMost Pole Position" << (pole_leaders.size() > 1 ? "s shared" : "") << ":\n";
        for (const auto& [driver, poles] : pole_leaders) {
            std::cout << "  " << driver << " (" << poles << " poles)\n";
        }
        
        auto win_leaders = race_performances.get_all_most_wins();
        std::cout << "\nMost Race Win" << (win_leaders.size() > 1 ? "s shared" : "") << ":\n";
        for (const auto& [driver, wins] : win_leaders) {
            std::cout << "  " << driver << " (" << wins << " wins)\n";
        }
        
        std::cout << std::string(60, '-') << "\n";
    }
    std::cout << *this;
    current_race++;
}

void Season::standings(const std::vector<std::pair<Driver*, long long>>& race_results) {
    auto& gameState = GameState::getInstance();
    const int points[] = {25, 18, 15, 12, 10, 8, 6, 4, 2, 1};
    
    for (size_t i = 0; i < race_results.size() && i < 10; i++) {
        const Driver* driver = race_results[i].first;
        driver_points[driver->get_name()] += points[i];
        
        for (const Team* team : teams) {
            if (team->get_driver_car(1).driver == driver || 
                team->get_driver_car(2).driver == driver) {
                team_points[team->get_name()] += points[i];
                gameState.recordRaceResult(team->get_name(), driver->get_name(), static_cast<int>(i) + 1);
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
        const auto& gameState = GameState::getInstance();
        os << "\nFINAL SEASON RESULTS\n";
        os << std::string(60, '*') << "\n";
        
        os << "\nDRIVER STANDINGS AND STATISTICS\n";
        os << std::string(60, '-') << "\n";
        int pos = 1;
        for (const auto& [driver_name, points] : driver_standings) {
            auto dStats = gameState.getDriverStats(driver_name);
            os << pos << ". " << driver_name << " - " << points << " points\n";
            os << "   Career Stats: "
               << dStats.driverChampionships << " Championships, "
               << dStats.raceWins << " Wins, "
               << dStats.podiums << " Podiums\n\n";
            pos++;
        }

        os << "\nTEAM STANDINGS AND STATISTICS\n";
        os << std::string(60, '-') << "\n";
        pos = 1;
        for (const auto& [team_name, points] : team_standings) {
            auto tStats = gameState.getTeamStats(team_name);
            os << pos << ". " << team_name << " - " << points << " points\n";
            os << "   History: "
               << tStats.constructorChampionships << " Championships, "
               << tStats.championshipWins << " Wins, "
               << tStats.podiums << " Podiums\n\n";
            pos++;
        }

        os << std::string(60, '*') << "\n";
    }
    
    return os;
}
void Season::recordSeasonChampions() {
    auto& gameState = GameState::getInstance();
    
    std::vector<std::pair<std::string, int>> driver_standings(
        driver_points.begin(),
        driver_points.end()
    );
    std::ranges::sort(driver_standings, [](const auto& a, const auto& b) {
        return a.second > b.second;
    });
    if (!driver_standings.empty()) {
        gameState.recordDriverChampion(driver_standings[0].first);
    }

    std::vector<std::pair<std::string, int>> team_standings(
        team_points.begin(),
        team_points.end()
    );
    std::ranges::sort(team_standings, [](const auto& a, const auto& b) {
        return a.second > b.second;
    });
    if (!team_standings.empty()) {
        gameState.recordConstructorChampion(team_standings[0].first);
    }
}