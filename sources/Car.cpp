#include "Car.h"

Car::Car(const int aerodynamics, const int powertrain, const int durability, const int chasis, Team* team)
    : aerodynamics(aerodynamics), powertrain(powertrain), durability(durability), chasis(chasis), team(team) {}

void Car::upgrade() {
    upgrades++;
    std::cout << "puncte upgrade: " << upgrades << std::endl;
    while (upgrades >= 5) {
        upgrades -= 5;
        apply_upgrades();
    }
}

void Car::downgrade() {
    downgrades++;
    std::cout << "puncte downgrade: " << downgrades << std::endl;
    while (downgrades >= 5) {
        downgrades -= 5;
        apply_downgrades();
    }
}

void Car::apply_upgrades() {
    upgrades = 0;
    aerodynamics += 2;
    powertrain += 1;
    durability += 1;
    chasis += 2;
    std::cout << "noul rating: " << rating() << std::endl;
}
void Car::apply_downgrades() {
    downgrades = 0;
    aerodynamics -= 2;
    powertrain -= 1;
    durability -= 1;
    chasis -= 2;
    std::cout << "noul rating: " << rating() << std::endl;
}
int Car::rating() const {
    return static_cast<int>(0.25* aerodynamics + 0.25 * powertrain + 0.25 * durability + 0.25 * chasis);
}

Car::Car(const Car& other) = default;
Car& Car::operator=(const Car& other) = default;
Car::~Car() = default;

int Car::get_rating() const {
    return rating();
}
void Car::set_team(Team *team) {
    this->team = team;
}

std::ostream& operator<<(std::ostream& os, const Car& obj) {
    os << "Aerodynamics: " << obj.aerodynamics << "\n"
       << "Powertrain: " << obj.powertrain << "\n"
       << "Durability: " << obj.durability << "\n"
       << "Chasis: " << obj.chasis << "\n";
    return os;
}

