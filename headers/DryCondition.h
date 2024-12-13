#ifndef DRYCONDITION_H
#define DRYCONDITION_H
#include "WeatherCondition.h"
#include "Team.h"

class DryCondition : public WeatherCondition {
public:
    DryCondition();
    [[nodiscard]] std::unique_ptr<WeatherCondition> clone() const override;
    DryCondition(const DryCondition& other);
    DryCondition& operator=(DryCondition rhs);
    friend void swap(DryCondition& first, DryCondition& second) noexcept;
    void apply_effects(Team* team) override;
    void remove_effects(Team* team) override;

private:
    void print_(std::ostream& os) const override;
};

#endif