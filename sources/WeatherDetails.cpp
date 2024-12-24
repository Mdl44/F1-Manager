#include "WeatherDetails.h"

int WeatherDetails::getBonus(int teamId) const {
    if (teamId >= 0 && static_cast<size_t>(teamId) < bonuses.size()) {
        return bonuses[teamId];
    }
    return 0;
}

void WeatherDetails::setBonuses(std::vector<int> team_bonuses) {
    bonuses = std::move(team_bonuses);
}

std::unique_ptr<WeatherDetails> DryWeatherDetails::clone() const {
    auto copy = std::make_unique<DryWeatherDetails>();
    copy->bonuses = bonuses;
    return copy;
}


std::unique_ptr<WeatherDetails> IntermediateWeatherDetails::clone() const {
    auto copy = std::make_unique<IntermediateWeatherDetails>();
    copy->bonuses = bonuses;
    return copy;
}


std::unique_ptr<WeatherDetails> WetWeatherDetails::clone() const {
    auto copy = std::make_unique<WetWeatherDetails>();
    copy->bonuses = bonuses;
    return copy;
}


std::unique_ptr<WeatherDetails> NightWeatherDetails::clone() const {
    auto copy = std::make_unique<NightWeatherDetails>();
    copy->bonuses = bonuses;
    return copy;
}
std::unique_ptr<WeatherDetails> MixedWeatherDetails::clone() const {
    auto copy = std::make_unique<MixedWeatherDetails>();
    copy->bonuses = bonuses;
    return copy;
}