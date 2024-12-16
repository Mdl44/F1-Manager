#include "WeatherConditionFactory.h"
#include "DryCondition.h"
#include "WetCondition.h"
#include "IntermediateCondition.h"
#include "NightCondition.h"
#include "MixedCondition.h"
#include <iostream>
#include <random>

std::unique_ptr<WeatherCondition> WeatherConditionFactory::getWeather(const bool isNight, const bool canRain, const bool quali) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    if (!canRain) {
        std::cout << (quali ? "Qualifying: " : "Race: ") << (isNight ? "Night" : "Dry") << " conditions\n";
        if (isNight) {
            return std::make_unique<NightCondition>();
        }
        return std::make_unique<DryCondition>();
    }

    std::uniform_int_distribution<> dis(0, 99);
    const auto weather = dis(gen);

    std::cout << (quali ? "Qualifying" : "Race") << " - Rain possible - randomly choosing weather...\n";

    if (isNight) {
        if (weather < 50) {
            std::cout << (quali ? "Qualifying" : "Race") << ": Night (Dry)\n";
            return std::make_unique<NightCondition>();
        } else if (weather < 70) {
            std::cout << (quali ? "Qualifying" : "Race") << ": Night + Mixed\n";
            return std::make_unique<MixedCondition>();
        } else if (weather < 85) {
            std::cout << (quali ? "Qualifying" : "Race") << ": Night + Intermediate\n";
            return std::make_unique<IntermediateCondition>();
        } else {
            std::cout << (quali ? "Qualifying" : "Race") << ": Night + Wet\n";
            return std::make_unique<WetCondition>();
        }
    } else {
        if (weather < 50) {
            std::cout << (quali ? "Qualifying" : "Race") << ": Dry\n";
            return std::make_unique<DryCondition>();
        } else if (weather < 70) {
            std::cout << (quali ? "Qualifying" : "Race") << ": Mixed\n";
            return std::make_unique<MixedCondition>();
        } else if (weather < 85) {
            std::cout << (quali ? "Qualifying" : "Race") << ": Intermediate\n";
            return std::make_unique<IntermediateCondition>();
        } else {
            std::cout << (quali ? "Qualifying" : "Race") << ": Wet\n";
            return std::make_unique<WetCondition>();
        }
    }
}