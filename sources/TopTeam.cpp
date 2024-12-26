#include "TopTeam.h"

TopTeam::TopTeam(const int id,
                 std::string name,
                 std::unique_ptr<Car> car1,
                 std::unique_ptr<Car> car2,
                 std::unique_ptr<Driver> driver1,
                 std::unique_ptr<Driver> driver2,
                 std::unique_ptr<Driver> reserve1,
                 std::unique_ptr<Driver> reserve2,
                 const int initial_position)
    : Team(id,
           std::move(name),
           std::move(car1),
           std::move(car2),
           std::move(driver1),
           std::move(driver2),
           std::move(reserve1),
           std::move(reserve2),
           initial_position) {}

TopTeam& TopTeam::operator=(const TopTeam& other) {
    if (this != &other) {
        Team::operator=(other);
    }
    return *this;
}

int TopTeam::getInfrastructureBonus() const {
    Driver_Car pair1 = get_driver_car(1);
    Driver_Car pair2 = get_driver_car(2);
    
    float car_rating = 0.0f;
    if (pair1.car) {
        const auto perf = pair1.car->get_performance();
        car_rating = (perf.aerodynamics + perf.chasis + perf.powertrain + perf.durability) / 4.0f;
    }
    
    float driver1_rating = pair1.driver ? pair1.driver->get_performance().overall_rating : 0.0f;
    float driver2_rating = pair2.driver ? pair2.driver->get_performance().overall_rating : 0.0f;
    
    float total_rating = (car_rating * 0.5f) + (driver1_rating * 0.3f) + (driver2_rating * 0.2f);
    
    return std::max(1, std::min(5, static_cast<int>(total_rating / 20.0f)));
}