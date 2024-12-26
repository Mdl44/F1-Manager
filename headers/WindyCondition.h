#ifndef WINDYCONDITION_H
#define WINDYCONDITION_H
#include "WeatherCondition.h"
#include <random>
#include <unordered_map>
#include "TopTeam.h"

class WindyCondition : public WeatherCondition {
public:
    WindyCondition();
    [[nodiscard]] std::unique_ptr<WeatherCondition> clone() const override;
    void apply_effects(Team* team) override;
    void remove_effects(Team* team) override;

private:
    void print_(std::ostream& os) const override;
    std::unordered_map<Team*, int> team_wind_bonuses;
    std::unordered_map<Driver*, int> driver_wind_bonuses;

protected:
    [[nodiscard]] int team_bonus(const Car* car) const override;
    [[nodiscard]] int driver_bonus(const Driver* driver) const override;
};
#endif