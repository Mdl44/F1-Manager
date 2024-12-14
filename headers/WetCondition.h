#ifndef WETCONDITION_H
#define WETCONDITION_H
#include "WeatherCondition.h"
#include <random>
#include <unordered_map>
#include "Driver.h"

class WetCondition : public WeatherCondition {
public:
    WetCondition();
    [[nodiscard]] std::unique_ptr<WeatherCondition> clone() const override;
    WetCondition(const WetCondition& other);
     WetCondition& operator=(WetCondition rhs);
    friend void swap(WetCondition& first, WetCondition& second) noexcept;
    void apply_effects(Team* team) override;
    void remove_effects(Team* team) override;

private:
    void print_(std::ostream& os) const override;
    std::unordered_map<Team*, int> team_mistake_factors;
    std::unordered_map<Driver*, int> driver_mistakes;
};

#endif