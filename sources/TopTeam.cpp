#include "TopTeam.h"

TopTeam::TopTeam(const std::string& name, std::unique_ptr<Car> car1, std::unique_ptr<Car> car2,std::unique_ptr<Driver> driver1, std::unique_ptr<Driver> driver2,
                 const int initial_position,const int dry_bonus,const int intermediate_bonus,const int wet_bonus,const int night_bonus)
    : Team(name, std::move(car1), std::move(car2),std::move(driver1), std::move(driver2),initial_position, dry_bonus, intermediate_bonus,wet_bonus, night_bonus) {}

TopTeam::TopTeam(const TopTeam& other) : Team(other) {}

TopTeam& TopTeam::operator=(const TopTeam& other) {
    if (this != &other) {
        Team::operator=(other);
    }
    return *this;
}
int TopTeam::getInfrastructureBonus() {
    return 15;
}
