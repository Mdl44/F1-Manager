#include "RaceWeekend.h"
#include <iostream>
#include <algorithm>
#include <random>
#include "Exceptions.h"

RaceWeekend::RaceWeekend(std::string name, const int laps, const int reference_time, const bool rain, const bool night_race) 
    : laps(laps), reference_time(reference_time), rain(rain), night_race(night_race) {
    if (name.empty()) {
        throw RaceWeekendException("Invalid race configuration: Race name cannot be empty");
    }
    this->name = std::move(name);

    if (laps <= 0) {
        throw RaceWeekendException("Invalid race configuration: Lap count must be positive");
    }
    if (reference_time <= 0) {
        throw RaceWeekendException("Invalid race configuration: Reference time must be positive");
    }
}

void RaceWeekend::set_quali_weather(const std::unique_ptr<WeatherCondition> &weather) {
    quali_weather = weather ? weather->clone() : nullptr;
}

void RaceWeekend::set_race_weather(const std::unique_ptr<WeatherCondition> &weather) {
    race_weather = weather ? weather->clone() : nullptr;
}

int random_time_generator(const int maxOffset = 500) { 
    static std::default_random_engine generator(std::random_device{}()); 
    std::uniform_int_distribution<int> distribution(-maxOffset, maxOffset); 
    return distribution(generator); 
}

RaceWeekend::~RaceWeekend() { 
    std::cout << "Deleting RaceWeekend " << name << std::endl; 
} 

RaceWeekend::RaceWeekend(const RaceWeekend& other) 
    : name(other.name), 
      laps(other.laps), 
      reference_time(other.reference_time), 
      rain(other.rain), 
      night_race(other.night_race),
      quali_results(other.quali_results), 
      race_results(other.race_results),
      quali_weather(nullptr),
      race_weather(nullptr),
      teams(other.teams) 
{
    if (other.quali_weather) {
        quali_weather = other.quali_weather->clone();
    }

    if (other.race_weather) {
        race_weather = other.race_weather->clone();
    }
}

RaceWeekend& RaceWeekend::operator=(RaceWeekend other) {
    swap(*this, other);
    return *this;
}

void swap(RaceWeekend& first, RaceWeekend& second) noexcept {
    using std::swap;
    swap(first.name, second.name);
    swap(first.laps, second.laps);
    swap(first.reference_time, second.reference_time);
    swap(first.rain, second.rain);
    swap(first.night_race, second.night_race);
    swap(first.quali_results, second.quali_results);
    swap(first.race_results, second.race_results);
    swap(first.quali_weather, second.quali_weather);
    swap(first.race_weather, second.race_weather);
    swap(first.teams, second.teams);
}


/*
void RaceWeekend::print_pre_weather_stats() const {
    std::cout << "\n=== Pre-Weather Stats ===\n";
    if (!teams.empty()) {
        for (const auto* team : teams) {
            std::cout << "Team: " << team->get_name() << "\n";
            Driver_Car pair1 = team->get_driver_car(1);
            Driver_Car pair2 = team->get_driver_car(2);

            if (pair1.driver && pair1.car) {
                std::cout << "Driver 1: " << pair1.driver->get_name()
                         << " (Rating: " << pair1.driver->get_performance().overall_rating << ")"
                         << " Car Rating: " << pair1.car->get_performance().overall_rating << "\n";
            }

            if (pair2.driver && pair2.car) {
                std::cout << "Driver 2: " << pair2.driver->get_name()
                         << " (Rating: " << pair2.driver->get_performance().overall_rating << ")"
                         << " Car Rating: " << pair2.car->get_performance().overall_rating << "\n";
            }
            std::cout << "\n";
        }
    }
}

void RaceWeekend::print_post_weather_stats() const {
    std::cout << "\n=== Post-Weather Stats ===\n";
    for (const auto* team : teams) {
        Driver_Car pair1 = team->get_driver_car(1);
        Driver_Car pair2 = team->get_driver_car(2);
        std::cout << "Team: " << team->get_name() << "\n";
        std::cout << "D1: " << (pair1.driver ? pair1.driver->get_name() : "None")
                  << " (" << (pair1.driver ? pair1.driver->get_performance().overall_rating : 0) << ")"
                  << " Car: " << (pair1.car ? pair1.car->get_performance().overall_rating : 0) << "\n";
        std::cout << "D2: " << (pair2.driver ? pair2.driver->get_name() : "None")
                  << " (" << (pair2.driver ? pair2.driver->get_performance().overall_rating : 0) << ")"
                  << " Car: " << (pair2.car ? pair2.car->get_performance().overall_rating : 0) << "\n\n";
    }
}
*/
void RaceWeekend::apply_weather_effects(const std::unique_ptr<WeatherCondition>& weather) const {
    if (weather) {
        for (const auto* team : teams) {
            weather->apply_effects(const_cast<Team*>(team));
        }
    }
}

void RaceWeekend::remove_weather_effects(const std::unique_ptr<WeatherCondition>& weather) const {
    if (weather) {
        for (const auto* team : teams) {
            weather->remove_effects(const_cast<Team*>(team));
        }
    }
}

std::pair<double, double> RaceWeekend::calculate_performance_factors(const Driver* driver) const {
    const double performance_factor = (driver->get_performance().overall_rating / 100.0);
    double car_rating = 0.0;
    
    for (const auto *team : teams) {
        Driver_Car pair1 = team->get_driver_car(1);
        Driver_Car pair2 = team->get_driver_car(2);
        
        if (driver == pair1.driver) {
            car_rating = static_cast<double>(pair1.car->get_performance().overall_rating);
            break;
        }
        if (driver == pair2.driver) {
            car_rating = static_cast<double>(pair2.car->get_performance().overall_rating);
            break;
        }
    }
    
    return {performance_factor, car_rating / 100.0};
}

void RaceWeekend::quali(const std::vector<std::pair<Driver*, int>>& drivers) {
    //print_pre_weather_stats();
    apply_weather_effects(quali_weather);
    //print_post_weather_stats();

    quali_results.clear();
    for (const auto& [driver, rating] : drivers) {
        auto [performance_factor, car_factor] = calculate_performance_factors(driver);
        
        long long time = reference_time;
        time += static_cast<long long>((1.0 - performance_factor) * 800);
        time += static_cast<long long>((1.0 - car_factor) * 1200);
        time += random_time_generator(200);

        if (quali_weather) {
            time += quali_weather->get_lap_time_modifier();
        }

        quali_results.emplace_back(driver, time);
    }

    remove_weather_effects(quali_weather);

    std::ranges::sort(quali_results, [](const auto& lhs, const auto& rhs) {
        return lhs.second < rhs.second;
    });
}

std::vector<std::pair<Driver*, long long>> RaceWeekend::race() {
    //print_pre_weather_stats();
    apply_weather_effects(race_weather);
    //print_post_weather_stats();

    race_results.clear();
    for (size_t i = 0; i < quali_results.size(); ++i) {
        auto& [driver, quali_time] = quali_results[i];
        const long long start_penalty = static_cast<long long>(i) * 500;
        
        auto [performance_factor, car_factor] = calculate_performance_factors(driver);
        
        long long total_time = start_penalty + 1000000;
        for (int lap = 1; lap <= laps; lap++) {
            long long lap_time = reference_time;
            lap_time += static_cast<long long>((1.0 - performance_factor) * 600);
            lap_time += static_cast<long long>((1.0 - car_factor) * 900);
            lap_time += random_time_generator(100);
            lap_time += 2000;

            if (race_weather) {
                lap_time += race_weather->get_lap_time_modifier();
            }

            record_lap_time(driver, lap_time, lap);
            total_time += lap_time;
        }

        race_results.emplace_back(driver, total_time);
    }

    remove_weather_effects(race_weather);

    std::ranges::sort(race_results, [](const auto& lhs, const auto& rhs) {
        return lhs.second < rhs.second;
    });

    return race_results;
}

void RaceWeekend::printResults(std::ostream& os, const std::vector<std::pair<Driver*, long long>>& results, const bool isRace) {
    if (results.empty()) return;

    int pos = 1;
    const long long leader_time = results[0].second;

    for (const auto& [driver, time] : results) {
        const int hours = isRace ? static_cast<int>(time / (1000 * 60 * 60)) : 0;
        const int minutes = static_cast<int>((time % (1000 * 60 * 60)) / (1000 * 60));
        const int seconds = static_cast<int>((time % (1000 * 60)) / 1000);
        const int milliseconds = static_cast<int>(time % 1000);

        std::string pos_str = (pos < 10 ? " " : "") + std::to_string(pos) + ".";
        std::string driver_name = driver->get_name();
        if (driver_name.length() < 25) {
            driver_name += std::string(25 - driver_name.length(), ' ');
        }

        os << pos_str << " " << driver_name;
        if (isRace && hours > 0) {
            os << hours << ":";
        }
        os << minutes << ":"
           << (seconds < 10 ? "0" : "") << seconds << "."
           << (milliseconds < 100 ? "0" : "")
           << (milliseconds < 10 ? "0" : "")
           << milliseconds;

        if (pos > 1) {
            const long long gap_ms = time - leader_time;
            std::string gap_str = " (+";
            gap_str += std::to_string(gap_ms / 1000);
            gap_str += ".";
            const long long ms = gap_ms % 1000;
            if (ms < 100) gap_str += "0";
            if (ms < 10) gap_str += "0";
            gap_str += std::to_string(ms);
            gap_str += "s)";
            os << gap_str;
        }

        os << "\n";
        pos++;
    }
    os << std::string(60, '-') << "\n";
}

std::ostream& operator<<(std::ostream& os, const RaceWeekend& weekend) {
    os << "\n=== " << weekend.name << " Race Weekend ===\n";

    os << "\nQualifying Weather:\n";
    if (weekend.quali_weather) {
        os << *weekend.quali_weather;
    } else {
        os << "No weather condition set\n";
    }

    os << "\nRace Weather:\n";
    if (weekend.race_weather) {
        os << *weekend.race_weather;
    } else {
        os << "No weather condition set\n";
    }

    os << "\nCircuit Info:\n"
       << "Laps: " << weekend.laps << "\n"
       << "Reference Time: " << weekend.reference_time << "\n"
       << "Night Race: " << (weekend.night_race ? "Yes" : "No") << "\n"
       << "Can Rain: " << (weekend.rain ? "Yes" : "No") << "\n";

    if (!weekend.quali_results.empty()) {
        os << "\nQualifying Results:\n";
        os << std::string(60, '-') << "\n";
        RaceWeekend::printResults(os, weekend.quali_results, false);
    }

    if (!weekend.race_results.empty()) {
        os << "\nRace Results:\n";
        os << std::string(60, '-') << "\n";
        RaceWeekend::printResults(os, weekend.race_results, true);
    }
    return os;
}
bool RaceWeekend::can_rain() const {
    return rain;
}
bool RaceWeekend::night() const {
    return night_race;
}
const std::string& RaceWeekend::get_name() const { 
    return name; 
}
void RaceWeekend::set_teams(const std::vector<Team *> &race_teams) {
    teams = race_teams;
}
const std::vector<std::pair<Driver*, long long>>& RaceWeekend::get_quali_results() const {
        return quali_results;
}

void RaceWeekend::record_lap_time(Driver* driver, long long time, int lap) {
    lap_times.emplace_back(driver, time, lap);
}

const std::vector<std::tuple<Driver*, long long, int>>& RaceWeekend::get_lap_times() const {
    return lap_times;
}