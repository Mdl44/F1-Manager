#ifndef MIXEDCONDITION_H
#define MIXEDCONDITION_H
#include "WeatherCondition.h"
#include <random>
#include <unordered_map>

class MixedCondition : public WeatherCondition {
public:
    MixedCondition();
    [[nodiscard]] std::unique_ptr<WeatherCondition> clone() const override;
    void apply_effects(Team* team) override;
    void remove_effects(Team* team) override;

private:
    void do_print_(std::ostream& os) const override;
    std::unordered_map<Team*, int> team_mixed_bonuses;
    std::unordered_map<Driver*, int> driver_mixed_bonuses;
protected:
    [[nodiscard]] int team_bonus(const Car* car) const override;
    [[nodiscard]] int driver_bonus(const Driver* driver) const override;
};
#endif