#include "DryCondition.h"

DryCondition::DryCondition() : WeatherCondition("Dry", 0) {}

std::unique_ptr<WeatherCondition> DryCondition::clone() const {
    return std::make_unique<DryCondition>(*this);
}

DryCondition::DryCondition(const DryCondition& other) : WeatherCondition(other) {}

DryCondition& DryCondition::operator=(const DryCondition& other) {
    if (this != &other) {
        WeatherCondition::operator=(other);
    }
    return *this;
}

void DryCondition::print_(std::ostream& os) const {
    WeatherCondition::print_(os);
    os << "Dry racing conditions - standard performance\n";
}

void DryCondition::apply_effects(Team* team) {
    if (const auto* t = dynamic_cast<Team*>(team)) {
        const int team_bonus = t->get_dry_bonus();
        t->get_car1()->apply_race_upgrade(team_bonus);
        t->get_car2()->apply_race_upgrade(team_bonus);
        
        if (t->get_driver1()) {
            t->get_driver1()->apply_race_upgrade(t->get_driver1()->get_dry_skill());
        }
        if (t->get_driver2()) {
            t->get_driver2()->apply_race_upgrade(t->get_driver2()->get_dry_skill());
        }
    }
}

void DryCondition::remove_effects(Team* team) {
    if (const auto* t = dynamic_cast<Team*>(team)) {
        const int team_bonus = t->get_dry_bonus();
        t->get_car1()->remove_race_upgrade(team_bonus);
        t->get_car2()->remove_race_upgrade(team_bonus);
        
        if (t->get_driver1()) {
            t->get_driver1()->remove_race_upgrade(t->get_driver1()->get_dry_skill());
        }
        if (t->get_driver2()) {
            t->get_driver2()->remove_race_upgrade(t->get_driver2()->get_dry_skill());
        }
    }
}