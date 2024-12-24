#ifndef WEATHER_DETAILS_H
#define WEATHER_DETAILS_H

#include <vector>
#include <memory>
#include "WeatherTypes.h"

class WeatherDetails {
protected:
    std::vector<int> bonuses;

public:
    virtual ~WeatherDetails() = default;
    [[nodiscard]] int getBonus(int teamId) const;
    void setBonuses(std::vector<int> team_bonuses);
    [[nodiscard]] virtual Weather_types getType() const = 0;
    [[nodiscard]] virtual std::unique_ptr<WeatherDetails> clone() const = 0;
};

class DryWeatherDetails : public WeatherDetails {
public:
    [[nodiscard]] Weather_types getType() const override;
    [[nodiscard]] std::unique_ptr<WeatherDetails> clone() const override;
};

class IntermediateWeatherDetails : public WeatherDetails {
public:
    [[nodiscard]] Weather_types getType() const override;
    [[nodiscard]] std::unique_ptr<WeatherDetails> clone() const override;
};

class WetWeatherDetails : public WeatherDetails {
public:
    [[nodiscard]] Weather_types getType() const override;
    [[nodiscard]] std::unique_ptr<WeatherDetails> clone() const override;
};

class NightWeatherDetails : public WeatherDetails {
public:
    [[nodiscard]] Weather_types getType() const override;
    [[nodiscard]] std::unique_ptr<WeatherDetails> clone() const override;
};

#endif