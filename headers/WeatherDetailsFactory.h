#ifndef WEATHER_DETAILS_FACTORY_H
#define WEATHER_DETAILS_FACTORY_H

#include "WeatherDetails.h"
#include <memory>
#include <unordered_map>
#include "WeatherTypes.h"

class WeatherDetailsFactory {
public:
    static std::unordered_map<Weather_types, std::unique_ptr<WeatherDetails>> createAll();
};

#endif