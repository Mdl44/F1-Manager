#ifndef HOTCONDITION_H
#define HOTCONDITION_H
#include "WeatherCondition.h"
#include <random>
#include <unordered_map>

class HotCondition : public WeatherCondition {
public:
    HotCondition();
    [[nodiscard]] std::unique_ptr<WeatherCondition> clone() const override;
    void apply_effects(Team* team) override;
    void remove_effects(Team* team) override;

private:
    void print_(std::ostream& os) const override;
    std::unordered_map<Team*, int> team_heat_impacts{};
protected:
    [[nodiscard]] int team_bonus(const Car* car) const override;
    [[nodiscard]] int driver_bonus(const Driver*) const override;
};
#endif