#include "Driver.h"
#include <iostream>
#include "WeatherCondition.h"
#include "Exceptions.h"

Driver::Driver(std::string name, const int experience, const int race_craft, const int awareness, const int race_pace, const int age, const int dry_skill, const int intermediate_skill, const int wet_skill)
    :experience(experience), race_craft(race_craft), awareness(awareness), race_pace(race_pace), age(age), dry_skill(dry_skill), intermediate_skill(intermediate_skill), wet_skill(wet_skill) {
    this->value = market_value();

    if (name.empty()) {
        throw InvalidDriverException("Driver name cannot be empty");
    }
    this->name = std::move(name);
    if (experience < 0 || experience > 100 || 
        race_craft < 0 || race_craft > 100 ||
        awareness < 0 || awareness > 100 ||
        race_pace < 0 || race_pace > 100) {
        throw InvalidDriverException("Driver stats must be between 0 and 100");
    }
    if (age < 16 || age > 50) {
        throw InvalidDriverException("Invalid driver age: " + std::to_string(age));
    }
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
    value(other.value),
    age(other.age),
    dry_skill(other.dry_skill),
    intermediate_skill(other.intermediate_skill),
    wet_skill(other.wet_skill){
}

Driver& Driver::operator=(const Driver& other) {
    if (this != &other) {
        name = other.name;
        experience = other.experience;
        race_craft = other.race_craft;
        awareness = other.awareness;
        race_pace = other.race_pace;
        value = other.value;
        age = other.age;
        dry_skill = other.dry_skill;
        intermediate_skill = other.intermediate_skill;
        wet_skill = other.wet_skill;
    }
    return *this;
}

Driver::~Driver() {
    std::cout << "Destructor driver: " << name << std::endl;
}

const std::string& Driver::get_name() const {
    return name;
}

int Driver::get_skill(const Weather_types& condition) const {
    switch (condition) {
        case Weather_types::DRY:
            return dry_skill;
        case Weather_types::INTERMEDIATE:
            return intermediate_skill; 
        case Weather_types::WET:
            return wet_skill;
        case Weather_types::NIGHT:
            return dry_skill;
        default:
            std::cerr << "Invalid weather condition" << std::endl;
            return 0;
    }
}

DriverPerformance Driver::get_performance() const {
    return {
        rating(),
        market_value(),
        experience
    };
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
void Driver::apply_race_upgrade(const int value_) {
    experience += value_;
    race_craft += value_;
    awareness += value_;
    race_pace += value_;
}

void Driver::remove_race_upgrade(const int value_) {
    experience -= value_;
    race_craft -= value_;
    awareness -= value_;
    race_pace -= value_;
}

std::ostream& operator<<(std::ostream& os, const Driver& driver) {
    os << "Name: " << driver.name << "\nExperience: " << driver.experience
       << "\nRace Craft: " << driver.race_craft << "\nAwareness: " << driver.awareness
       << "\nPace: " << driver.race_pace << "\nRating: " << driver.rating()
       << "\nValue: " << driver.value;
    return os;
}