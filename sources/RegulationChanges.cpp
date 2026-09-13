#include "RegulationChanges.h"
#include "Team.h"

int RegulationChanges::calculate_reduction(const int stats) {
    if (stats >= 200) return static_cast<int>(stats * 0.20);
    if (stats >= 150) return static_cast<int>(stats * 0.15);
    if (stats >= 100) return static_cast<int>(stats * 0.10);
    return static_cast<int>(stats * 0.05);
}

std::vector<std::string> RegulationChanges::apply_regulation_changes(const std::vector<std::unique_ptr<Team>>& teams) {
    std::vector<std::string> events = {"REGULATION CHANGES\n"};

    for (const auto& team : teams) {
        events.push_back("\nTeam: " + team->get_name() + "\n");

        for (int i = 1; i <= 2; i++) {
            if (auto driver_car = team->get_driver_car(i); driver_car.car) {
                const int initial_rating = driver_car.car->get_performance().overall_rating;
                const int downgrades = calculate_reduction(initial_rating);

                driver_car.car->apply_downgrades(downgrades);

                events.push_back("Car " + std::to_string(i) + " Rating: " + std::to_string(initial_rating) +
                                  " -> " + std::to_string(driver_car.car->get_performance().overall_rating) + "\n");
            }
        }

        for (int i = 1; i <= 2; i++) {
            if (auto driver_car = team->get_driver_car(i); driver_car.driver) {
                const int initial_rating = driver_car.driver->get_performance().overall_rating;
                const int downgrades = calculate_reduction(initial_rating);

                driver_car.driver->apply_downgrades(downgrades);

                events.push_back("Driver " + std::to_string(i) + " Rating: " + std::to_string(initial_rating) +
                                  " -> " + std::to_string(driver_car.driver->get_performance().overall_rating) + "\n");
            }
        }
    }

    return events;
}
