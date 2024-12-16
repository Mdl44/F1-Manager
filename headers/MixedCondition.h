#ifndef MIXEDCONDITION_H
#define MIXEDCONDITION_H
#include "WeatherCondition.h"
#include <unordered_map>
#include "Driver.h"

class MixedCondition : public WeatherCondition {
public:
    MixedCondition();
    [[nodiscard]] std::unique_ptr<WeatherCondition> clone() const override;
    void apply_effects(Team* team) override;
    void remove_effects(Team* team) override;

private:
    void print_(std::ostream& os) const override;
    std::unordered_map<Team*, int> team_mixed_bonuses;
    std::unordered_map<Driver*, int> driver_mixed_bonuses;
};

#endif