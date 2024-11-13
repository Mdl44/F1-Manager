#include "Car.h"

#include "Team.h"

Car::Car(const int aerodynamics, const int powertrain, const int durability, const int chasis, Team* team)
    : aerodynamics(aerodynamics), powertrain(powertrain), durability(durability), chasis(chasis), team(team) {}

void Car::upgrade() {
    if (team && team->is_player_controlled()) {
        upgrades++;
    } else {
        upgrades++;
        while (upgrades >= 5) {
            upgrades -= 5;
            apply_upgrades();
        }
    }
}

void Car::downgrade() {
    downgrades++;
    std::cout << "downgrade points: " << downgrades << std::endl;
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
    std::cout << "new rating: " << rating() << std::endl;
}
void Car::apply_downgrades() {
    downgrades = 0;
    aerodynamics -= 2;
    powertrain -= 1;
    durability -= 1;
    chasis -= 2;
    std::cout << "new rating: " << rating() << std::endl;
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
void Car::set_team(Team *team_set) {
    this->team = team_set;
}

std::ostream& operator<<(std::ostream& os, const Car& obj) {
    os << "Aerodynamics: " << obj.aerodynamics << "\n"
       << "Powertrain: " << obj.powertrain << "\n"
       << "Durability: " << obj.durability << "\n"
       << "Chasis: " << obj.chasis << "\n";
    return os;
}

