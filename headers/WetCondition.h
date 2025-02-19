#ifndef WETCONDITION_H
#define WETCONDITION_H
#include "WeatherCondition.h"
#include <random>
#include <unordered_map>

class WetCondition : public WeatherCondition {
public:
    WetCondition();
    [[nodiscard]] std::unique_ptr<WeatherCondition> clone() const override;
    void apply_effects(Team* team) override;
    void remove_effects(Team* team) override;

private:
    void do_print_(std::ostream& os) const override;
    std::unordered_map<Team*, int> team_mistake_factors;
    std::unordered_map<Driver*, int> driver_mistakes;
    
protected:
    [[nodiscard]] int team_bonus(const Car* car) const override;
    [[nodiscard]] int driver_bonus(const Driver* driver) const override;
    
};
#endif