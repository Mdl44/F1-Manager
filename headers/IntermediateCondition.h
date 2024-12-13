#ifndef INTERMEDIATECONDITION_H
#define INTERMEDIATECONDITION_H
#include "WeatherCondition.h"

class IntermediateCondition : public WeatherCondition {
public:
    IntermediateCondition();
    [[nodiscard]] std::unique_ptr<WeatherCondition> clone() const override;
    IntermediateCondition(const IntermediateCondition& other);
    IntermediateCondition& operator=(IntermediateCondition rhs);
    friend void swap(IntermediateCondition& first, IntermediateCondition& second) noexcept;
    void apply_effects(Team* team) override;
    void remove_effects(Team* team) override;

private:
    void print_(std::ostream& os) const override;
};

#endif