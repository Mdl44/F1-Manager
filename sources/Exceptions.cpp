#include "Exceptions.h"

ConfigurationFileException::ConfigurationFileException(const std::string& file_path)
    : Exceptions("Failed to load configuration file: " + file_path) {}

InvalidDriverException::InvalidDriverException(const std::string& message)
    : Exceptions("Driver error: " + message) {}

InvalidTeamException::InvalidTeamException(const std::string& message)
    : Exceptions("Team error: " + message) {}

WeatherSimulationException::WeatherSimulationException(const std::string& weather_type)
    : Exceptions("Weather simulation error for condition: " + weather_type) {}

RaceWeekendException::RaceWeekendException(const std::string& race_name)
    : Exceptions("Race weekend error at: " + race_name) {}