#include "Driver.h"
#include <iostream>

Driver::Driver(std::string name, const int experience, const int race_craft, const int awareness, const int race_pace, Car* car, Team* team)
    : name(std::move(name)), experience(experience), race_craft(race_craft), awareness(awareness), race_pace(race_pace), car(car), team(team) {
    value = market_value();
}

int Driver::rating() const {
    return static_cast<int>(0.25 * experience + 0.25 * race_craft + 0.25 * awareness + 0.25 * race_pace);
}

float Driver::market_value() const {
    return static_cast<float>(rating() - 55);
}

Driver::Driver(const Driver& other) = default;
Driver& Driver::operator=(const Driver& other) = default;

Driver::~Driver() {
    std::cout << "destructor pilot: " << name << std::endl;
}

std::string& Driver::get_name() {
    return name;
}

int Driver::get_rating() const {
    return rating();
}

float Driver::get_market_value() const {
    return value;
}

void Driver::set_car(Car* car_set) {
    this->car = car_set;
}

void Driver::set_team(Team* team_set) {
    this->team = team_set;
}

Car* Driver::get_car() const {
    return car;
}

Team* Driver::get_team() const {
    return team;
}

std::ostream& operator<<(std::ostream& os, const Driver& driver) {
    os << "Name: " << driver.name << "\nExperience: " << driver.experience
       << "\nRace Craft: " << driver.race_craft << "\nAwareness: " << driver.awareness
       << "\nPace: " << driver.race_pace << "\nRating: " << driver.rating()
       << "\nValue: " << driver.value;
    return os;
}
