#include "IntermediateCondition.h"
#include "Team.h"
#include <iostream>

    IntermediateCondition::IntermediateCondition(): WeatherCondition("Intermediate", 20000) {}
    
    std::unique_ptr<WeatherCondition> IntermediateCondition::clone() const{
        return std::make_unique<IntermediateCondition>(*this);
    }

   void IntermediateCondition::apply_effects(Team* team) {
    if (const auto* t = team) {
        const int team_bonus = t->getWeatherBonus(Weather_types::INTERMEDIATE);

        Driver_Car pair1 = t->get_driver_car(1);
        pair1.car->apply_upgrades(team_bonus);
        if (pair1.driver) {
            const int exp_bonus = pair1.driver->get_performance().experience > 75 ? 15 : 5;
            const int skill = pair1.driver->get_skill(Weather_types::INTERMEDIATE);
            pair1.driver->apply_upgrades(skill + exp_bonus);
        }

        Driver_Car pair2 = t->get_driver_car(2);
        pair2.car->apply_upgrades(team_bonus);
        if (pair2.driver) {
            const int exp_bonus = pair2.driver->get_performance().experience > 75 ? 15 : 5;
            const int skill = pair2.driver->get_skill(Weather_types::INTERMEDIATE);
            pair2.driver->apply_upgrades(skill + exp_bonus);
        }
    }
}

    void IntermediateCondition::remove_effects(Team* team) {
    if (const auto* t = team) {
        const int team_bonus = t->getWeatherBonus(Weather_types::INTERMEDIATE);

        Driver_Car pair1 = t->get_driver_car(1);
        pair1.car->apply_downgrades(team_bonus);
        if (pair1.driver) {
            const int exp_bonus = pair1.driver->get_performance().experience > 75 ? 15 : 5;
            const int skill = pair1.driver->get_skill(Weather_types::INTERMEDIATE);
            pair1.driver->apply_downgrades(skill + exp_bonus);
        }

        Driver_Car pair2 = t->get_driver_car(2);
        pair2.car->apply_downgrades(team_bonus);
        if (pair2.driver) {
            const int exp_bonus = pair2.driver->get_performance().experience > 75 ? 15 : 5;
            const int skill = pair2.driver->get_skill(Weather_types::INTERMEDIATE);
            pair2.driver->apply_downgrades(skill + exp_bonus);
        }
    }
}
void IntermediateCondition::print_(std::ostream& os) const {
    os << "Intermediate conditions - experience matters\n"
       << "Veteran drivers have an advantage\n";
}