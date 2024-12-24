#include "WeatherDetails.h"
#include "Exceptions.h"
#include <fstream>

void WeatherDetails::readFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file) {
        throw ConfigurationFileException(filepath);
    }

    std::string line;
    bool found_section = false;
    while (std::getline(file, line)) {
        if (line == getWeatherTypeString()) {
            found_section = true;
            continue;
        }
        if (found_section) {
        int current_bonus = 0;
        for (const char c : line) {
        if (c >= '0' && c <= '9') {
            current_bonus = current_bonus * 10 + (c - '0');
        } else if (current_bonus > 0) {
            break;
        }
    }
    setBonus(current_bonus);
    break;
}
    }
}

std::unique_ptr<WeatherDetails> DryWeatherDetails::clone() const {
    auto copy = std::make_unique<DryWeatherDetails>();
    copy->bonus = bonus;
    return copy;
}

std::string DryWeatherDetails::getWeatherTypeString() const { 
    return "DRY"; 
}

std::unique_ptr<WeatherDetails> IntermediateWeatherDetails::clone() const {
    auto copy = std::make_unique<IntermediateWeatherDetails>();
    copy->bonus = bonus;
    return copy;
}

std::string IntermediateWeatherDetails::getWeatherTypeString() const { 
    return "INTERMEDIATE"; 
}

std::unique_ptr<WeatherDetails> WetWeatherDetails::clone() const {
    auto copy = std::make_unique<WetWeatherDetails>();
    copy->bonus = bonus;
    return copy;
}

std::string WetWeatherDetails::getWeatherTypeString() const { 
    return "WET"; 
}

std::unique_ptr<WeatherDetails> NightWeatherDetails::clone() const {
    auto copy = std::make_unique<NightWeatherDetails>();
    copy->bonus = bonus;
    return copy;
}

std::string NightWeatherDetails::getWeatherTypeString() const { 
    return "NIGHT"; 
}