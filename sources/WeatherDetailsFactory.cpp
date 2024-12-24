#include "WeatherDetailsFactory.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Exceptions.h"

std::unordered_map<Weather_types, std::vector<int>> WeatherDetailsFactory::weather_bonuses;

void WeatherDetailsFactory::initialize() {
    std::ifstream file("weather_details.txt");
    if (!file) {
        throw ConfigurationFileException("weather_details.txt");
    }

    std::string weather_type;
    while (std::getline(file, weather_type)) {
        std::string line;
        if (std::getline(file, line)) {
            std::stringstream ss(line);
            std::vector<int> bonuses;
            int bonus;
            while (ss >> bonus) {
                bonuses.push_back(bonus);
            }

            if (weather_type == "DRY") {
                weather_bonuses[Weather_types::DRY] = bonuses;
            } else if (weather_type == "INTERMEDIATE") {
                weather_bonuses[Weather_types::INTERMEDIATE] = bonuses;
            } else if (weather_type == "WET") {
                weather_bonuses[Weather_types::WET] = bonuses;
            } else if (weather_type == "NIGHT") {
                weather_bonuses[Weather_types::NIGHT] = bonuses;
            }
        }
    }
    std::cout << "\nWeather bonuses loaded:\n";
    for (const auto& [type, bonuses] : weather_bonuses) {
        std::cout << "Type: ";
        switch (type) {
            case Weather_types::DRY: std::cout << "DRY"; break;
            case Weather_types::INTERMEDIATE: std::cout << "INTERMEDIATE"; break;
            case Weather_types::WET: std::cout << "WET"; break;
            case Weather_types::NIGHT: std::cout << "NIGHT"; break;
        }
        std::cout << "\nBonuses: ";
        for (int b : bonuses) {
            std::cout << b << " ";
        }
        std::cout << "\n";
    }
}

std::unordered_map<Weather_types, std::unique_ptr<WeatherDetails>> 
WeatherDetailsFactory::createForTeam(int teamId) {
    std::unordered_map<Weather_types, std::unique_ptr<WeatherDetails>> details;
    
    auto dry = std::make_unique<DryWeatherDetails>();
    auto intermediate = std::make_unique<IntermediateWeatherDetails>();
    auto wet = std::make_unique<WetWeatherDetails>();
    auto night = std::make_unique<NightWeatherDetails>();

    dry->setBonuses(weather_bonuses[Weather_types::DRY]);
    intermediate->setBonuses(weather_bonuses[Weather_types::INTERMEDIATE]);
    wet->setBonuses(weather_bonuses[Weather_types::WET]); 
    night->setBonuses(weather_bonuses[Weather_types::NIGHT]);

    if (teamId >= 0 && static_cast<size_t>(teamId) < weather_bonuses[Weather_types::DRY].size()) {
        details[Weather_types::DRY] = std::move(dry);
        details[Weather_types::INTERMEDIATE] = std::move(intermediate);
        details[Weather_types::WET] = std::move(wet);
        details[Weather_types::NIGHT] = std::move(night);
    }

    return details;
}