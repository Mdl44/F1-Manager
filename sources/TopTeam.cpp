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

int TopTeam::getInfrastructureBonus() {
    return 3;
}