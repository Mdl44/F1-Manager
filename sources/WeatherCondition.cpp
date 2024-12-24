#include "WeatherCondition.h"
#include <iostream>

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
    obj.print_(os);
    return os;
}
