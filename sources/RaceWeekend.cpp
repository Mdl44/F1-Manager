#include "RaceWeekend.h"
#include "Car.h"
#include <algorithm>
#include <random>

RaceWeekend::RaceWeekend(std::string name, int laps, int reference_time)
    : name(std::move(name)), laps(laps), reference_time(reference_time) {}


int random_time_generator(int maxOffset = 200) {
    static std::default_random_engine generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(-maxOffset, maxOffset);
    return distribution(generator);
}

void RaceWeekend::quali(const std::vector<Driver*>& drivers) {
    for (auto driver : drivers) {
        int driver_rating = driver->get_rating();
        int car_rating = driver->get_car()->get_rating();
        int combined = (driver_rating + car_rating) / 2;
        int performance_factor = 100 - combined;
        long long time = reference_time + (performance_factor * 3) + random_time_generator();
        quali_results.emplace_back(driver, time);
    }
    std::ranges::sort(quali_results, [](const auto& lhs, const auto& rhs) {
        return lhs.second < rhs.second;
    });
}

std::vector<std::pair<Driver*, long long>> RaceWeekend::race() {
    race_results.clear();
    for (size_t i = 0; i < quali_results.size(); ++i) {
        auto& driver = quali_results[i].first;
        long long start_delay = static_cast<long long>(i) * 500;

        int combined = (driver->get_rating() + driver->get_car()->get_rating()) / 2;
        int performance_factor = 100 - combined;
        long long lap_time = reference_time + (performance_factor * 3);

        long long total_time = start_delay;
        for (int lap = 0; lap < laps; ++lap) {
            total_time += lap_time + random_time_generator();
        }
        race_results.emplace_back(driver, total_time);
    }
    std::ranges::sort(race_results, [](const auto& lhs, const auto& rhs) {
        return lhs.second < rhs.second;
    });

    return race_results;
}

void RaceWeekend::display_quali() const {
    std::cout << "\nQualifying Results - " << name << "\n";
    std::cout << std::string(60, '-') << "\n";
    int pos = 1;
    for (const auto& [driver, time] : quali_results) {
        std::string pos_str = (pos < 10 ? " " : "") + std::to_string(pos) + ".";

        int minutes = static_cast<int>(time / (1000 * 60));
        int seconds = static_cast<int>((time % (1000 * 60)) / 1000);
        int milliseconds = static_cast<int>(time % 1000);

        std::string driver_name = driver->get_name();
        if (driver_name.length() < 25) {
            driver_name += std::string(25 - driver_name.length(), ' ');
        }
        std::cout << pos_str << " "
                  << driver_name
                  << minutes << ":"
                  << (seconds < 10 ? "0" : "") << seconds << "."
                  << (milliseconds < 100 ? "0" : "") << (milliseconds < 10 ? "0" : "") << milliseconds << "\n";
        pos++;
    }
    std::cout << std::string(60, '-') << "\n\n";
}

void RaceWeekend::display_race() const {
    std::cout << "\nRace Results - " << name << "\n";
    std::cout << std::string(60, '-') << "\n";
    int pos = 1;
    for (const auto& [driver, time] : race_results) {
        std::string pos_str = (pos < 10 ? " " : "") + std::to_string(pos) + ".";

        int hours = static_cast<int>(time / (1000 * 60 * 60));
        int minutes = static_cast<int>((time % (1000 * 60 * 60)) / (1000 * 60));
        int seconds = static_cast<int>((time % (1000 * 60)) / 1000);
        int milliseconds = static_cast<int>(time % 1000);

        std::string driver_name = driver->get_name();
        if (driver_name.length() < 25) {
            driver_name += std::string(25 - driver_name.length(), ' ');
        }
        std::cout << pos_str << " "
                  << driver_name
                  << hours << ":"
                  << (minutes < 10 ? "0" : "") << minutes << ":"
                  << (seconds < 10 ? "0" : "") << seconds << "."
                  << (milliseconds < 100 ? "0" : "") << (milliseconds < 10 ? "0" : "") << milliseconds << "\n";
        pos++;
    }
    std::cout << std::string(60, '-') << "\n\n";
}
std::string RaceWeekend::get_name() const {
    return name;
}
const std::vector<std::pair<Driver*, long long>>& RaceWeekend::get_quali_results() const {
    return quali_results;
}
