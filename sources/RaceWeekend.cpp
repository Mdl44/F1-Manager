#include "RaceWeekend.h"
#include <iostream>
#include <algorithm>
#include <random>

RaceWeekend::RaceWeekend(std::string name, const int laps, const int reference_time)
    : name(std::move(name)), laps(laps), reference_time(reference_time) {}

int random_time_generator(const int maxOffset = 200) {
        static std::default_random_engine generator(std::random_device{}());
        std::uniform_int_distribution<int> distribution(-maxOffset, maxOffset);
        return distribution(generator);
    }

RaceWeekend::~RaceWeekend() {
    std::cout << "Deleting RaceWeekend " << name << std::endl;
}
RaceWeekend::RaceWeekend(const RaceWeekend& other)
    : name(other.name), laps(other.laps), reference_time(other.reference_time) {
    quali_results = other.quali_results;
    race_results = other.race_results;
}
RaceWeekend& RaceWeekend::operator=(const RaceWeekend& other) {
    if (this == &other) {
        return *this;
    }
    name = other.name;
    laps = other.laps;
    reference_time = other.reference_time;
    quali_results = other.quali_results;
    race_results = other.race_results;

    return *this;
}

void RaceWeekend::quali(const std::vector<std::pair<Driver*, int>>& combined_ratings) {
    quali_results.clear();
    for (const auto& [driver, rating] : combined_ratings) {
        int performance_factor = 0;
        if (rating < 100) performance_factor = 100 - rating;
        else if (rating < 150) performance_factor = 150 - rating;
        else if (rating < 200) performance_factor = 200 - rating;
        else performance_factor = 250 - rating;
        
        long long time = reference_time + (performance_factor * 3) + random_time_generator();
        quali_results.emplace_back(driver, time);
    }
    
    std::ranges::sort(quali_results, [](const auto& lhs, const auto& rhs) {
        return lhs.second < rhs.second;
    });
}

const std::string &RaceWeekend::get_name() const
{
    return name;
}

std::vector<std::pair<Driver*, long long>> RaceWeekend::race() {
    race_results.clear();
    
    for (size_t i = 0; i < quali_results.size(); ++i) {
        auto& [driver, quali_time] = quali_results[i];
        const long long start_delay = static_cast<long long>(i) * 500;
        
        const int driver_rating = driver->get_rating();
        int performance_factor = 0;
        if (driver_rating < 100) performance_factor = 100 - driver_rating;
        else if (driver_rating < 150) performance_factor = 150 - driver_rating;
        else if (driver_rating < 200) performance_factor = 200 - driver_rating;
        else performance_factor = 250 - driver_rating;
        
        const long long lap_time = reference_time + (performance_factor * 3);
        long long total_time = start_delay;
        
        for (int lap = 0; lap < laps; ++lap) {
            total_time += lap_time + random_time_generator() + (420000/laps);
        }
        
        race_results.emplace_back(driver, total_time);
    }
    
    std::ranges::sort(race_results, [](const auto& lhs, const auto& rhs) {
        return lhs.second < rhs.second;
    });
    
    return race_results;
}

std::ostream& operator<<(std::ostream& os, const RaceWeekend& weekend) {
    os << "\n=== " << weekend.name << " Race Weekend ===\n";
    os << "Laps: " << weekend.laps << "\n";
    os << "Reference Time: " << weekend.reference_time << "\n";
    
    if (!weekend.quali_results.empty()) {
        os << "\nQualifying Results:\n";
        os << std::string(60, '-') << "\n";
        int pos = 1;
        for (const auto& [driver, time] : weekend.quali_results) {
            const int minutes = static_cast<int>(time / (1000 * 60));
            const int seconds = static_cast<int>((time % (1000 * 60)) / 1000);
            const int milliseconds = static_cast<int>(time % 1000);

            std::string pos_str = (pos < 10 ? " " : "") + std::to_string(pos) + ".";
            std::string driver_name = driver->get_name();
            if (driver_name.length() < 25) {
                driver_name += std::string(25 - driver_name.length(), ' ');
            }
            
            os << pos_str << " " << driver_name 
               << minutes << ":" 
               << (seconds < 10 ? "0" : "") << seconds << "."
               << (milliseconds < 100 ? "0" : "") 
               << (milliseconds < 10 ? "0" : "") 
               << milliseconds << "\n";
            pos++;
        }
        os << std::string(60, '-') << "\n";
    }

    if (!weekend.race_results.empty()) {
        os << "\nRace Results:\n";
        os << std::string(60, '-') << "\n";
        int pos = 1;
        for (const auto& [driver, time] : weekend.race_results) {
            const int hours = static_cast<int>(time / (1000 * 60 * 60));
            const int minutes = static_cast<int>((time % (1000 * 60 * 60)) / (1000 * 60));
            const int seconds = static_cast<int>((time % (1000 * 60)) / 1000);
            const int milliseconds = static_cast<int>(time % 1000);

            std::string pos_str = (pos < 10 ? " " : "") + std::to_string(pos) + ".";
            std::string driver_name = driver->get_name();
            if (driver_name.length() < 25) {
                driver_name += std::string(25 - driver_name.length(), ' ');
            }
            
            os << pos_str << " " << driver_name 
               << hours << ":" 
               << (minutes < 10 ? "0" : "") << minutes << ":"
               << (seconds < 10 ? "0" : "") << seconds << "."
               << (milliseconds < 100 ? "0" : "") 
               << (milliseconds < 10 ? "0" : "") 
               << milliseconds << "\n";
            pos++;
        }
        os << std::string(60, '-') << "\n";
    }
    
    return os;
}