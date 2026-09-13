#include "Driver.h"
#include <iostream>
#include "WeatherCondition.h"
#include "Exceptions.h"
#include "GameRules.h"

Driver::Driver(std::string name, const int experience, const int race_craft, const int awareness, const int race_pace, const int age)
    :experience(experience), race_craft(race_craft), awareness(awareness), race_pace(race_pace), age(age) {
    this->value = market_value();

    if (name.empty()) {
        throw InvalidDriverException("Driver name cannot be empty");
    }
    this->name = std::move(name);
    if (experience < GameRules::Driver::MIN_STAT || experience > GameRules::Driver::MAX_STAT ||
        race_craft < GameRules::Driver::MIN_STAT || race_craft > GameRules::Driver::MAX_STAT ||
        awareness < GameRules::Driver::MIN_STAT || awareness > GameRules::Driver::MAX_STAT ||
        race_pace < GameRules::Driver::MIN_STAT || race_pace > GameRules::Driver::MAX_STAT) {
        throw InvalidDriverException("Driver stats must be between 0 and 100");
    }
    if (age < GameRules::Driver::MIN_AGE || age > GameRules::Driver::MAX_AGE) {
        throw InvalidDriverException("Invalid driver age: " + std::to_string(age));
    }
}

int Driver::rating() const {
    return static_cast<int>(0.25 * experience + 0.25 * race_craft + 0.25 * awareness + 0.25 * race_pace);
}

float Driver::market_value() const {
    return static_cast<float>(rating()) - GameRules::Driver::MARKET_VALUE_BASELINE;
}

Driver::Driver(const Driver& other) :
    name(other.name),
    experience(other.experience),
    race_craft(other.race_craft),
    awareness(other.awareness),
    race_pace(other.race_pace),
    value(other.value),
    age(other.age)
{
    if (experience < GameRules::Driver::MIN_STAT || experience > GameRules::Driver::MAX_STAT ||
        race_craft < GameRules::Driver::MIN_STAT || race_craft > GameRules::Driver::MAX_STAT ||
        awareness < GameRules::Driver::MIN_STAT || awareness > GameRules::Driver::MAX_STAT ||
        race_pace < GameRules::Driver::MIN_STAT || race_pace > GameRules::Driver::MAX_STAT ||
        age < GameRules::Driver::MIN_AGE || age > GameRules::Driver::MAX_AGE) {
        throw InvalidDriverException("Invalid driver stats in copy constructor");
    }
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
    }
    return *this;
}

Driver::~Driver() = default;

const std::string& Driver::get_name() const {
    return name;
}

DriverPerformance Driver::get_performance() const {
    return {
        rating(),market_value(),experience,race_craft,awareness,race_pace};
}
void Driver::apply_upgrades(const int value_) {
    experience += value_;
    race_craft += value_;
    awareness += value_;
    race_pace += value_;
    value = market_value();
}

void Driver::apply_downgrades(const int value_) {
    experience -= value_;
    race_craft -= value_;
    awareness -= value_;
    race_pace -= value_;
    value = market_value();
}

std::ostream& operator<<(std::ostream& os, const Driver& driver) {
    os << "Name: " << driver.name << "\nExperience: " << driver.experience
       << "\nRace Craft: " << driver.race_craft << "\nAwareness: " << driver.awareness
       << "\nPace: " << driver.race_pace << "\nRating: " << driver.rating()
       << "\nValue: " << driver.value;
    return os;
}
void Driver::increase_age() {
    age++;
    if (age > GameRules::Driver::MAX_AGE) {
        throw InvalidDriverException("Driver is too old");
    }
}
int Driver::get_age() const {
    return age;
}