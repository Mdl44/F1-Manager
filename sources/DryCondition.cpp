#include "DryCondition.h"

DryCondition::DryCondition() : WeatherCondition("Dry", 0) {}

std::unique_ptr<WeatherCondition> DryCondition::clone() const {
    return std::make_unique<DryCondition>(*this);
}

void DryCondition::print_(std::ostream& os) const {
    os << "Dry racing conditions - standard performance\n";
}

void DryCondition::apply_effects(Team* team) {
    if (const auto* t = team) {
        const int team_bonus = t->getWeatherBonus(Weather_types::DRY);

        Driver_Car pair1 = t->get_driver_car(1);
        pair1.car->apply_upgrades(team_bonus);
        if (pair1.driver) {
            pair1.driver->apply_upgrades(pair1.driver->get_skill(Weather_types::DRY));
        }

        Driver_Car pair2 = t->get_driver_car(2);
        pair2.car->apply_upgrades(team_bonus);
        if (pair2.driver) {
            pair2.driver->apply_upgrades(pair2.driver->get_skill(Weather_types::DRY));
        }
    }
}

void DryCondition::remove_effects(Team* team) {
    if (const auto* t = team) {
        const int team_bonus = t->getWeatherBonus(Weather_types::DRY);

        Driver_Car pair1 = t->get_driver_car(1);
        pair1.car->apply_downgrades(team_bonus);
        if (pair1.driver) {
            pair1.driver->apply_downgrades(pair1.driver->get_skill(Weather_types::DRY));
        }

        Driver_Car pair2 = t->get_driver_car(2);
        pair2.car->apply_downgrades(team_bonus);
        if (pair2.driver) {
            pair2.driver->apply_downgrades(pair2.driver->get_skill(Weather_types::DRY));
        }
    }
}