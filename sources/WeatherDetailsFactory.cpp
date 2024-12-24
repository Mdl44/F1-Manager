#include "WeatherDetailsFactory.h"
#include "WeatherDetails.h"

std::unordered_map<Weather_types, std::unique_ptr<WeatherDetails>> WeatherDetailsFactory::createAll() {
    std::unordered_map<Weather_types, std::unique_ptr<WeatherDetails>> allDetails;
    
    allDetails[Weather_types::DRY] = std::make_unique<DryWeatherDetails>();
    allDetails[Weather_types::INTERMEDIATE] = std::make_unique<IntermediateWeatherDetails>();
    allDetails[Weather_types::WET] = std::make_unique<WetWeatherDetails>();
    allDetails[Weather_types::NIGHT] = std::make_unique<NightWeatherDetails>();
    
    for (auto& weatherPair : allDetails) {
        std::unique_ptr<WeatherDetails>& weatherDetail = weatherPair.second;
        if (weatherDetail != nullptr) {
            weatherDetail->readFromFile("weather_details.txt");
        }
    }
    
    return allDetails;
}