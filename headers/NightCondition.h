#ifndef NIGHTCONDITION_H
#define NIGHTCONDITION_H
#include "WeatherCondition.h"
#include <random>
#include "TopTeam.h"

class NightCondition : public WeatherCondition {
public:
    NightCondition();
    [[nodiscard]] std::unique_ptr<WeatherCondition> clone() const override;
    NightCondition(const NightCondition& other);
    NightCondition& operator=(NightCondition rhs);
    friend void swap(NightCondition& first, NightCondition& second) noexcept;
    void apply_effects(Team* team) override;
    void remove_effects(Team* team) override;

private:
    void print_(std::ostream& os) const override;
};

#endif