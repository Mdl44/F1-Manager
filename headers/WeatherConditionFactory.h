#ifndef WEATHERCONDITIONFACTORY_H
#define WEATHERCONDITIONFACTORY_H

#include "WeatherCondition.h"
#include <memory>

class WeatherConditionFactory {
public:
    static std::unique_ptr<WeatherCondition> getWeather(bool isNight, bool canRain);
};

#endif