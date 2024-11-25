#include "Car.h"

Car::Car(const int aerodynamics, const int powertrain, const int durability, const int chasis)
    : aerodynamics(aerodynamics), powertrain(powertrain), durability(durability), chasis(chasis) {}

int Car::rating() const {
    return static_cast<int>(0.25* aerodynamics + 0.25 * powertrain + 0.25 * durability + 0.25 * chasis);
}

Car::Car(const Car& other) = default;
Car& Car::operator=(const Car& other) = default;
Car::~Car() {
    std::cout << "destructor car" << std::endl;
}

int Car::get_rating() const {
    return rating();
}

void Car::apply_upgrade() {
    aerodynamics += 1;
    powertrain += 1;
    durability += 1;
    chasis += 1;
}

void Car::apply_downgrade() {
    aerodynamics -= 1;
    powertrain -= 1;
    durability -= 1;
    chasis -= 1;
}

std::ostream& operator<<(std::ostream& os, const Car& obj) {
    os << "Car Stats:\n"
       << "Aerodynamics: " << obj.aerodynamics << "\n"
       << "Powertrain: " << obj.powertrain << "\n"
       << "Durability: " << obj.durability << "\n"
       << "Chassis: " << obj.chasis << "\n"
       << "Overall Rating: " << obj.rating() << "\n";
    return os;
}