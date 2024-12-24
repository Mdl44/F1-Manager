#include "WeatherDetails.h"

int WeatherDetails::getBonus(int teamId) const {
    if (teamId >= 0 && teamId < bonuses.size()) {
        return bonuses[teamId];
    }
    return 0;
}

void WeatherDetails::setBonuses(std::vector<int> team_bonuses) {
    bonuses = std::move(team_bonuses);
}

Weather_types DryWeatherDetails::getType() const {
    return Weather_types::DRY;
}

std::unique_ptr<WeatherDetails> DryWeatherDetails::clone() const {
    auto copy = std::make_unique<DryWeatherDetails>();
    copy->bonuses = bonuses;
    return copy;
}

Weather_types IntermediateWeatherDetails::getType() const {
    return Weather_types::INTERMEDIATE;
}

std::unique_ptr<WeatherDetails> IntermediateWeatherDetails::clone() const {
    auto copy = std::make_unique<IntermediateWeatherDetails>();
    copy->bonuses = bonuses;
    return copy;
}

Weather_types WetWeatherDetails::getType() const {
    return Weather_types::WET;
}

std::unique_ptr<WeatherDetails> WetWeatherDetails::clone() const {
    auto copy = std::make_unique<WetWeatherDetails>();
    copy->bonuses = bonuses;
    return copy;
}

Weather_types NightWeatherDetails::getType() const {
    return Weather_types::NIGHT;
}

std::unique_ptr<WeatherDetails> NightWeatherDetails::clone() const {
    auto copy = std::make_unique<NightWeatherDetails>();
    copy->bonuses = bonuses;
    return copy;
}