#ifndef WEATHER_DETAILS_FACTORY_H
#define WEATHER_DETAILS_FACTORY_H

#include <unordered_map>
#include <memory>
#include "WeatherDetails.h"
#include "WeatherTypes.h"

class WeatherDetailsFactory {
    static std::unordered_map<Weather_types, std::vector<int>> weather_bonuses;

public:
    static void initialize();
    static std::unordered_map<Weather_types, std::unique_ptr<WeatherDetails>> createForTeam(int teamId);
};

#endif