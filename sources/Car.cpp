#include "Car.h"

Car::Car(const int aerodynamics, const int powertrain, const int durability, const int chasis, Team* team)
    : aerodynamics(aerodynamics), powertrain(powertrain), durability(durability), chasis(chasis), team(team) {}

int Car::rating() const {
    return static_cast<int>(0.25* aerodynamics + 0.25 * powertrain + 0.25 * durability + 0.25 * chasis);
}

Car::Car(const Car& other) = default;
Car& Car::operator=(const Car& other) = default;
Car::~Car() = default;

int Car::get_rating() const {
    return rating();
}
void Car::set_team(Team *team_set) {
    this->team = team_set;
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

