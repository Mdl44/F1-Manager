#include "Season.h"
#include <algorithm>
#include <iterator>
#include <sstream>
#include "Exceptions.h"
#include "WeatherConditionFactory.h"
#include "Stats.h"
#include "GameRules.h"

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
Season::~Season() = default;

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
        fastest_lap_driver = other.fastest_lap_driver;
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

RaceOutcome Season::race(RaceWeekend& weekend) {
    RaceOutcome outcome;

    std::vector<std::pair<Driver*, int>> combined_ratings;
    weekend.set_teams(teams);
    weekend.setup_weather();
    

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
    std::ostringstream weekend_stream;
    weekend_stream << weekend;
    outcome.weekend_report = weekend_stream.str();

    const auto race_laps = weekend.get_lap_times();
    auto [fastest_driver, fastest_time, lap_number] = find_fastest_lap<Driver*>(race_laps);
    fastest_lap_driver = fastest_driver;

    const int minutes = static_cast<int>((fastest_time % (1000 * 60 * 60)) / (1000 * 60));
    const int seconds = static_cast<int>((fastest_time % (1000 * 60)) / 1000);
    const int milliseconds = static_cast<int>(fastest_time % 1000);

    std::ostringstream lap_stream;
    lap_stream << "\nFastest Lap:\n";
    lap_stream << fastest_driver->get_name() << " - "
              << minutes << ":"
              << (seconds < 10 ? "0" : "") << seconds << "."
              << (milliseconds < 100 ? "0" : "")
              << (milliseconds < 10 ? "0" : "")
              << milliseconds
              << " (Lap " << lap_number << ")\n";
    outcome.fastest_lap_report = lap_stream.str();

    outcome.team_events = standings(results);

    if (current_race == races) {
        std::ostringstream analysis_stream;
        for (const auto& event : recordSeasonChampions()) {
            analysis_stream << event;
        }

        analysis_stream << "\nSEASON PERFORMANCE ANALYSIS\n";
        analysis_stream << std::string(60, '-') << "\n";

        auto best_cars = car_ratings.get_all_highest();
        analysis_stream << "Best Car" << (best_cars.size() > 1 ? "s" : "") << ":\n";
        for (const auto& [car, rating] : best_cars) {
            analysis_stream << "  " << car << " (Rating: " << rating << ")\n";
        }

        auto best_drivers = driver_ratings.get_all_highest();
        analysis_stream << "\nTop Driver Rating" << (best_drivers.size() > 1 ? "s" : "") << ":\n";
        for (const auto& [driver, rating] : best_drivers) {
            analysis_stream << "  " << driver << " (Rating: " << rating << ")\n";
        }

        auto pole_leaders = quali_performances.get_all_most_wins();
        analysis_stream << "\nMost Pole Position" << (pole_leaders.size() > 1 ? "s shared" : "") << ":\n";
        for (const auto& [driver, poles] : pole_leaders) {
            analysis_stream << "  " << driver << " (" << poles << " poles)\n";
        }

        auto win_leaders = race_performances.get_all_most_wins();
        analysis_stream << "\nMost Race Win" << (win_leaders.size() > 1 ? "s shared" : "") << ":\n";
        for (const auto& [driver, wins] : win_leaders) {
            analysis_stream << "  " << driver << " (" << wins << " wins)\n";
        }

        analysis_stream << std::string(60, '-') << "\n";
        outcome.season_analysis = analysis_stream.str();
    }

    std::ostringstream standings_stream;
    standings_stream << *this;
    outcome.standings_report = standings_stream.str();

    current_race++;
    return outcome;
}

std::vector<std::string> Season::standings(const std::vector<std::pair<Driver*, long long>>& race_results) {
    auto& stats = Stats::getInstance();
    const auto& points = GameRules::Season::POINTS_TABLE;

    for (size_t i = 0; i < race_results.size() && i < std::size(points); i++) {
        const Driver* driver = race_results[i].first;
        int position_points = points[i];
        if (fastest_lap_driver == driver) {
            position_points += GameRules::Season::FASTEST_LAP_BONUS_POINT;
        }

        driver_points[driver->get_name()] += position_points;
        
        for (const Team* team : teams) {
            if (team->get_driver_car(1).driver == driver || 
                team->get_driver_car(2).driver == driver) {
                team_points[team->get_name()] += position_points;
                stats.recordRaceResult(team->get_name(), driver->get_name(), static_cast<int>(i) + 1);
                break;
            }
        }
    }
    return update_team_performance();
}

std::vector<std::string> Season::update_team_performance() {
    std::vector<std::string> events;
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
                    auto downgrade_events = team->apply_downgrade();
                    events.insert(events.end(), downgrade_events.begin(), downgrade_events.end());
                }

                if (team->is_player_controlled()) {
                    if (team->get_upgrade_points() > 0) {
                        events.push_back("Player Team has " + std::to_string(team->get_upgrade_points()) + " upgrade points available.\n");
                    }
                } else if (team->get_upgrade_points() >= GameRules::Team::AI_AUTO_UPGRADE_THRESHOLD) {
                    auto upgrade_events = team->apply_upgrade_for_ai_team();
                    events.insert(events.end(), upgrade_events.begin(), upgrade_events.end());
                    events.push_back("AI Team " + team->get_name() + " applied upgrades.\n");
                }
                break;
            }
        }
    }
    return events;
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
        const auto& stats = Stats::getInstance();
        os << "\nFINAL SEASON RESULTS\n";
        os << std::string(60, '*') << "\n";
        
        os << "\nDRIVER STANDINGS AND STATISTICS\n";
        os << std::string(60, '-') << "\n";
        int pos = 1;
        for (const auto& [driver_name, points] : driver_standings) {
            auto dStats = stats.getDriverStats(driver_name);
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
            auto tStats = stats.getTeamStats(team_name);
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
std::vector<std::string> Season::recordSeasonChampions() {
    std::vector<std::string> events;
    auto& stats = Stats::getInstance();

    std::vector<std::pair<std::string, int>> driver_standings(
        driver_points.begin(),
        driver_points.end()
    );
    std::ranges::sort(driver_standings, [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    if (!driver_standings.empty()) {
        stats.recordDriverChampion(driver_standings[0].first);

         for (size_t i = 0; i < GameRules::Season::CHAMPIONSHIP_BONUS_POSITIONS && i < driver_standings.size(); i++) {
            const std::string& driverName = driver_standings[i].first;
            for (Team* team : teams) {
                const auto *d1 = team->get_driver_car(1).driver;
                const auto *d2 = team->get_driver_car(2).driver;
                if ((d1 && d1->get_name() == driverName) ||
                    (d2 && d2->get_name() == driverName)) {
                    const int bonus = GameRules::Season::CHAMPIONSHIP_BONUS_POSITIONS - static_cast<int>(i);
                    team->update_performance_points(-bonus);
                    events.push_back(team->get_name() + " received " + std::to_string(bonus) +
                             " upgrade points for driver position " + std::to_string(i + 1) + "\n");
                    break;
                }
            }
        }
    }

    std::vector<std::pair<std::string, int>> team_standings(
        team_points.begin(),
        team_points.end()
    );
    std::ranges::sort(team_standings, [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    if (!team_standings.empty()) {
        stats.recordConstructorChampion(team_standings[0].first);
        for (size_t i = 0; i < GameRules::Season::CHAMPIONSHIP_BONUS_POSITIONS && i < team_standings.size(); i++) {
            const std::string& teamName = team_standings[i].first;
            for (Team* team : teams) {
                if (team->get_name() == teamName) {
                    const int bonus = GameRules::Season::CHAMPIONSHIP_BONUS_POSITIONS - static_cast<int>(i);
                    team->update_performance_points(-bonus);
                    events.push_back(team->get_name() + " received " + std::to_string(bonus) +
                             " upgrade points for constructor position " + std::to_string(i + 1) + "\n");
                    break;
                }
            }
        }
    }

    return events;
}