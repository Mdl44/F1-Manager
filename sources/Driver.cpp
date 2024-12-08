#include "Driver.h"
#include <iostream>

Driver::Driver(std::string name, const int experience, const int race_craft, const int awareness, const int race_pace)
    : name(std::move(name)), experience(experience), race_craft(race_craft), awareness(awareness), race_pace(race_pace) {
    this->value = market_value();
}

int Driver::rating() const {
    return static_cast<int>(0.25 * experience + 0.25 * race_craft + 0.25 * awareness + 0.25 * race_pace);
}

float Driver::market_value() const {
    return static_cast<float>(rating() - 55);
}

Driver::Driver(const Driver& other) : 
    name(other.name),
    experience(other.experience),
    race_craft(other.race_craft),
    awareness(other.awareness),
    race_pace(other.race_pace),
    value(other.value) {
}

Driver& Driver::operator=(const Driver& other) {
    if (this != &other) {
        name = other.name;
        experience = other.experience;
        race_craft = other.race_craft;
        awareness = other.awareness;
        race_pace = other.race_pace;
        value = other.value;
    }
    return *this;
}

Driver::~Driver() {
    std::cout << "Destructor driver: " << name << std::endl;
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

void Driver::apply_upgrade() {
    experience += 1;
    race_craft += 1;
    awareness += 1;
    race_pace += 1;
    value = market_value();
}

void Driver::apply_downgrade() {
    experience -= 1;
    race_craft -= 1;
    awareness -= 1;
    race_pace -= 1;
    value = market_value();
}

std::ostream& operator<<(std::ostream& os, const Driver& driver) {
    os << "Name: " << driver.name << "\nExperience: " << driver.experience
       << "\nRace Craft: " << driver.race_craft << "\nAwareness: " << driver.awareness
       << "\nPace: " << driver.race_pace << "\nRating: " << driver.rating()
       << "\nValue: " << driver.value;
    return os;
}