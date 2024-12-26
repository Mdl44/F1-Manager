#include "WeatherConditionFactory.h"
#include "DryCondition.h"
#include "WetCondition.h"
#include "IntermediateCondition.h"
#include "NightCondition.h"
#include <iostream>
#include <random>
#include "MixedCondition.h"
#include "WindyCondition.h"
#include "HotCondition.h"

std::unique_ptr<WeatherCondition> WeatherConditionFactory::getWeather(const bool isNight, const bool canRain, const bool quali) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    if (!canRain) {
        std::uniform_int_distribution dis(0, 99);
        const auto weather = dis(gen);
        
        std::cout << (quali ? "Qualifying: " : "Race: ");
        if (isNight) {
            std::cout << "Night conditions\n";
            return std::make_unique<NightCondition>();
        }
        if (weather < 30) {
            std::cout << "Hot conditions\n";
            return std::make_unique<HotCondition>();
        }
        std::cout << "Dry conditions\n";
        return std::make_unique<DryCondition>();
    }

    std::uniform_int_distribution dis(0, 99);
    const auto weather = dis(gen);

    std::cout << (quali ? "Qualifying" : "Race") << " - Rain possible - randomly choosing weather...\n";

    if (isNight) {
        if (weather < 30) {
            std::cout << (quali ? "Qualifying" : "Race") << ": Night (Dry)\n";
            return std::make_unique<NightCondition>();
        }
        if (weather < 45) {
            std::cout << (quali ? "Qualifying" : "Race") << ": Night + Windy\n";
            return std::make_unique<WindyCondition>();
        }
        if (weather < 60) {
            std::cout << (quali ? "Qualifying" : "Race") << ": Night + Mixed\n";
            return std::make_unique<MixedCondition>();
        }
        if (weather < 85) {
            std::cout << (quali ? "Qualifying" : "Race") << ": Night + Intermediate\n";
            return std::make_unique<IntermediateCondition>();
        }
        std::cout << (quali ? "Qualifying" : "Race") << ": Night + Wet\n";
        return std::make_unique<WetCondition>();
    }

    if (weather < 20) {
        std::cout << (quali ? "Qualifying" : "Race") << ": Hot\n";
        return std::make_unique<HotCondition>();
    }
    if (weather < 40) {
        std::cout << (quali ? "Qualifying" : "Race") << ": Dry\n";
        return std::make_unique<DryCondition>();
    }
    if (weather < 55) {
        std::cout << (quali ? "Qualifying" : "Race") << ": Windy\n";
        return std::make_unique<WindyCondition>();
    }
    if (weather < 70) {
        std::cout << (quali ? "Qualifying" : "Race") << ": Mixed\n";
        return std::make_unique<MixedCondition>();
    }
    if (weather < 85) {
        std::cout << (quali ? "Qualifying" : "Race") << ": Intermediate\n";
        return std::make_unique<IntermediateCondition>();
    }
    std::cout << (quali ? "Qualifying" : "Race") << ": Wet\n";
    return std::make_unique<WetCondition>();
}