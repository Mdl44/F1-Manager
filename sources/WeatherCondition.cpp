#include "WeatherCondition.h"
#include <iostream>
#include <utility>

WeatherCondition::WeatherCondition(std::string name, const int modifier) 
    : name(std::move(name)), lap_time_modifier(modifier) {}

void WeatherCondition::print(std::ostream& os) const {
    print_(os);
}

int WeatherCondition::get_lap_time_modifier() const {
    return lap_time_modifier;
}

const std::string& WeatherCondition::get_name() const {
    return name;
}

void WeatherCondition::print_(std::ostream& os) const {
    os << "Weather: " << name << "\n"
       << "Lap time modifier: " << lap_time_modifier << "ms\n";
}

std::ostream& operator<<(std::ostream& os, const WeatherCondition& obj) {
    obj.print(os);
    return os;
}

WeatherCondition::WeatherCondition(const WeatherCondition& other) 
    : name(other.name)
    , lap_time_modifier(other.lap_time_modifier){}

WeatherCondition& WeatherCondition::operator=(const WeatherCondition& other) {
    auto copy = other.clone();
    swap(*this, *copy);
    return *this;
}

void swap(WeatherCondition& first, WeatherCondition& second) noexcept {
    using std::swap;
    swap(first.lap_time_modifier, second.lap_time_modifier);
    swap(first.name, second.name);
}
