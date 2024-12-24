#include "RegulationChanges.h"
#include "Team.h"
#include <iostream>

int RegulationChanges::calculate_reduction(const int stats) {
    if (stats >= 200) return static_cast<int>(stats * 0.20);
    if (stats >= 150) return static_cast<int>(stats * 0.15);
    if (stats >= 100) return static_cast<int>(stats * 0.10);
    return static_cast<int>(stats * 0.05);
}

void RegulationChanges::apply_regulation_changes(const std::vector<std::unique_ptr<Team>>& teams) {
    std::cout << "REGULATION CHANGES" << std::endl;
    for (const auto& team : teams) {
        std::cout << "\nTeam: " << team->get_name() << "\n";

        for (int i = 1; i <= 2; i++) {
            if (auto driver_car = team->get_driver_car(i); driver_car.car) {
                const int initial_rating = driver_car.car->get_rating();
                const int downgrades = calculate_reduction(initial_rating);

                    driver_car.car->apply_downgrades(downgrades);
                
                std::cout << "Car " << i << " Rating: " << initial_rating 
                         << " -> " << driver_car.car->get_rating() << "\n";
            }
        }

        for (int i = 1; i <= 2; i++) {
            if (auto driver_car = team->get_driver_car(i); driver_car.driver) {
                const int initial_rating = driver_car.driver->get_performance().overall_rating;
                const int downgrades = calculate_reduction(initial_rating);

                    driver_car.driver->apply_downgrades(downgrades);
                
                std::cout << "Driver " << i << " Rating: " << initial_rating 
                         << " -> " << driver_car.driver->get_performance().overall_rating << "\n";
            }
        }
    }
}