#include "Car.h"
#include "Exceptions.h"

Car::Car(const int aerodynamics, const int powertrain, const int durability, const int chasis)
    : aerodynamics(aerodynamics), powertrain(powertrain), durability(durability), chasis(chasis) {
    if (aerodynamics < 0 || aerodynamics > 100 ||
        powertrain < 0 || powertrain > 100 ||
        durability < 0 || durability > 100 ||
        chasis < 0 || chasis > 100) {
        throw InvalidTeamException("Car stats must be between 0 and 100");
    }
}

int Car::rating() const {
    return static_cast<int>(0.25* aerodynamics + 0.25 * powertrain + 0.25 * durability + 0.25 * chasis);
}

Car::Car(const Car& other) :
    aerodynamics(other.aerodynamics),
    powertrain(other.powertrain),
    durability(other.durability),
    chasis(other.chasis) {
}

Car& Car::operator=(const Car& other) {
    if (this != &other) { 
        aerodynamics = other.aerodynamics;
        powertrain = other.powertrain;
        durability = other.durability;
        chasis = other.chasis;
    }
    return *this;
}

Car::~Car() {
    std::cout << "destructor car" << std::endl;
}

CarPerformance Car::get_performance() const {
    return {rating(), aerodynamics, powertrain, durability, chasis};
}

void Car::apply_upgrades(const int value) {
    aerodynamics += value;
    powertrain += value;
    durability += value;
    chasis += value;
}

void Car::apply_downgrades(const int value) {
    aerodynamics -= value;
    powertrain -= value;
    durability -= value;
    chasis -= value;
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

