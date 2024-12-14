#include "RaceWeekend.h"
#include <iostream>
#include <algorithm>
#include <random>
#include "Exceptions.h"

static double get_rating_multiplier(const int rating) {
    static const std::vector<std::pair<int, double>> valori = {
        {20, 4.0}, {40, 3.5}, {60, 3.0}, {80, 2.7}, {100, 2.4},
        {120, 2.1}, {140, 1.9}, {160, 1.7}, {180, 1.5}, {200, 1.35},
        {220, 1.2}, {240, 1.1}, {250, 1.05}, {260, 1.0}
    };
    for (const auto& [limita, multiplier] : valori) {
        if (rating <= limita) {
            return multiplier;
        }
    }
    return 1.0;
}


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
    : name(other.name), laps(other.laps), reference_time(other.reference_time), 
      rain(other.rain), night_race(other.night_race),
       quali_results(other.quali_results), race_results(other.race_results) {

}

RaceWeekend& RaceWeekend::operator=(const RaceWeekend& other) { 
    if (this == &other) { return *this; } 
    name = other.name; 
    laps = other.laps; 
    reference_time = other.reference_time; 
    quali_results = other.quali_results; 
    race_results = other.race_results;
    return *this;
}

void RaceWeekend::quali(const std::vector<std::pair<Driver*, int>>& drivers) { 
    quali_results.clear(); 
    for (const auto& [driver, rating] : drivers) {
        const double performance_factor = get_rating_multiplier(driver->get_performance().overall_rating);
        double car_rating = 0.0;
        for (const auto *team : teams) {
            Driver_Car pair1 = team->get_driver_car(1);
            Driver_Car pair2 = team->get_driver_car(2);
            
            if (driver == pair1.driver) {
                car_rating = static_cast<double>(pair1.car->get_rating());
                break;
            }
            if (driver == pair2.driver) {
                car_rating = static_cast<double>(pair2.car->get_rating());
                break;
            }
        }
        const double car_factor = get_rating_multiplier(static_cast<int>(car_rating));

        const auto driver_impact = static_cast<long long>(std::round(performance_factor * 3000.0));
        const auto car_impact = static_cast<long long>(std::round(car_factor * 4000.0));

        long long time = reference_time + driver_impact + car_impact +
                        random_time_generator() - 23000;

        if (quali_weather) {
            time += quali_weather->get_lap_time_modifier();
            for (const auto* team : teams) {
            quali_weather->apply_effects(const_cast<Team*>(team));
        }
        }

        quali_results.emplace_back(driver, time);
    }
        if(quali_weather){
            for(const auto* team : teams){
                quali_weather->remove_effects(const_cast<Team*>(team));
            }
        }

    std::ranges::sort(quali_results, [](const auto& lhs, const auto& rhs) {
        return lhs.second < rhs.second;
    });
}

const std::string& RaceWeekend::get_name() const { 
    return name; 
}

std::vector<std::pair<Driver*, long long>> RaceWeekend::race() {
    race_results.clear();
    for (size_t i = 0; i < quali_results.size(); ++i) {
        auto& [driver, quali_time] = quali_results[i];
        const long long start_delay = static_cast<long long>(i) * 500;

        const double performance_factor = get_rating_multiplier(driver->get_performance().overall_rating);

        double car_rating = 0.0;
        for (const auto *team : teams) {
            Driver_Car pair1 = team->get_driver_car(1);
            Driver_Car pair2 = team->get_driver_car(2);
            
            if (driver == pair1.driver) {
                car_rating = static_cast<double>(pair1.car->get_rating());
                break;
            }
            if (driver == pair2.driver) {
                car_rating = static_cast<double>(pair2.car->get_rating());
                break;
            }
        }
        const double car_factor = get_rating_multiplier(static_cast<int>(car_rating));

        const auto driver_impact = static_cast<long long>(std::round(performance_factor * 3000.0));
        const auto car_impact = static_cast<long long>(std::round(car_factor * 4000.0));

        const long long base_lap_time = reference_time + driver_impact + car_impact - 23000;

        long long total_time = start_delay;
        for (int lap = 0; lap < laps; lap++) {
            long long lap_time = base_lap_time;
            lap_time += random_time_generator() + (1020000/laps);

            if (race_weather) {
            lap_time += race_weather->get_lap_time_modifier();
            for (const auto* team : teams) {
            race_weather->apply_effects(const_cast<Team*>(team));
        }
        }

            total_time += lap_time;
        }

        race_results.emplace_back(driver, total_time);
    }
    if(race_weather){
            for(const auto* team : teams){
                race_weather->remove_effects(const_cast<Team*>(team));
            }
        }

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
