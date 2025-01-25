#ifndef NIGHTCONDITION_H
#define NIGHTCONDITION_H
#include "WeatherCondition.h"
#include <random>
#include <unordered_map>
#include "TopTeam.h"

class NightCondition : public WeatherCondition {
public:
    NightCondition();
    [[nodiscard]] std::unique_ptr<WeatherCondition> clone() const override;
    void apply_effects(Team* team) override;
    void remove_effects(Team* team) override;

private:
    void do_print_(std::ostream& os) const override;
    std::unordered_map<Team*, int> team_temp_impacts;
    std::unordered_map<Driver*, int> driver_temp_impacts;
protected:
    [[nodiscard]] int team_bonus(const Car* car) const override;
    [[nodiscard]] int driver_bonus(const Driver* driver) const override;
    
};
#endif