#include "RaceWeekend.h"
#include "Car.h"
#include <algorithm>
#include <random>
#include <utility>

RaceWeekend::RaceWeekend(std::string  name, const int laps, const int reference_time)
    : name(std::move(name)), laps(laps), reference_time(reference_time) {}

int random_time_generator(const int maxOffset = 200) {
    static std::default_random_engine generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(-maxOffset, maxOffset);
    return distribution(generator);
}
void RaceWeekend::quali(const std::vector<Driver*>& drivers) {
    for (const auto& driver : drivers) {
        if (!driver || !driver->get_car()) {
            continue;
        }
        const int driver_rating = driver->get_rating();
        const int car_rating = driver->get_car()->get_rating();
        const int combined = (driver_rating + car_rating) / 2;
        const int performance_factor = 100 - combined;
        int timeMs = reference_time + (performance_factor * 3) + random_time_generator();
        timeMs = std::max(0, timeMs);

        quali_results.emplace_back(driver, timeMs / 1000.0);
    }
    std::ranges::sort(quali_results, [](const auto& lhs, const auto& rhs) {
        return lhs.second < rhs.second;
    });
}

void RaceWeekend::race() {
    race_results.clear();

    for (size_t i = 0; i < quali_results.size(); ++i) {
        const auto& driver = quali_results[i].first;
        const long long start_delay = static_cast<long long>(i) * 500;

        const int combined = (driver->get_rating() + driver->get_car()->get_rating()) / 2;
        const int performance_factor = 100 - combined;
        const long long averageLapTime = reference_time + (performance_factor * 3);

        long long totalRaceTime = start_delay;
        for (int lap = 0; lap < laps; ++lap) {
            totalRaceTime += averageLapTime + random_time_generator();
        }

        race_results.emplace_back(driver, totalRaceTime);
    }

    std::ranges::sort(race_results, [](const auto& lhs, const auto& rhs) {
        return lhs.second < rhs.second;
    });
}

void RaceWeekend::display_quali() const {
    std::cout << "Qualifying Results for " << name << ":\n";
    for (const auto& [driver, time] : quali_results) {
        std::cout << driver->get_name() << " - " << time << " seconds\n";
    }
}

void RaceWeekend::display_race() const {
    std::cout << "Race Results for " << name << ":\n";
    for (const auto& [driver, time] : race_results) {
        std::cout << driver->get_name() << " - " << time << " ms\n";
    }
}

std::string RaceWeekend::get_name() const {
    return name;
}

int RaceWeekend::get_reference_time() const {
    return reference_time;
}

int RaceWeekend::get_laps() const {
    return laps;
}


