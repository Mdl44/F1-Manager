#include "WeatherConditionFactory.h"
#include "DryCondition.h"
#include "WetCondition.h"
#include "IntermediateCondition.h"
#include "NightCondition.h"
#include <random>
#include "MixedCondition.h"
#include "WindyCondition.h"
#include "HotCondition.h"

std::unique_ptr<WeatherCondition> WeatherConditionFactory::getWeather(const bool isNight, const bool canRain) {
     std::random_device rd;
     std::mt19937 gen(rd());

    if (!canRain) {
        std::uniform_int_distribution dis(0, 99);
        const auto weather = dis(gen);

        if (isNight) {
            return std::make_unique<NightCondition>();
        }
        if (weather < 30) {
            return std::make_unique<HotCondition>();
        }
        return std::make_unique<DryCondition>();
    }

    std::uniform_int_distribution dis(0, 99);
    const auto weather = dis(gen);

    if (isNight) {
        if (weather < 30) {
            return std::make_unique<NightCondition>();
        }
        if (weather < 45) {
            return std::make_unique<WindyCondition>();
        }
        if (weather < 60) {
            return std::make_unique<MixedCondition>();
        }
        if (weather < 85) {
            return std::make_unique<IntermediateCondition>();
        }
        return std::make_unique<WetCondition>();
    }

    if (weather < 20) {
        return std::make_unique<HotCondition>();
    }
    if (weather < 40) {
        return std::make_unique<DryCondition>();
    }
    if (weather < 55) {
        return std::make_unique<WindyCondition>();
    }
    if (weather < 70) {
        return std::make_unique<MixedCondition>();
    }
    if (weather < 85) {
        return std::make_unique<IntermediateCondition>();
    }
    return std::make_unique<WetCondition>();
}
