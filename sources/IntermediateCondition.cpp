#include "IntermediateCondition.h"
#include "Team.h"
#include <iostream>

    IntermediateCondition::IntermediateCondition(): WeatherCondition("Intermediate", 20000) {}
    
    std::unique_ptr<WeatherCondition> IntermediateCondition::clone() const{
        return std::make_unique<IntermediateCondition>(*this);
    }
    
    IntermediateCondition::IntermediateCondition(const IntermediateCondition& other) : WeatherCondition(other) {}

    void swap(IntermediateCondition& first, IntermediateCondition& second) noexcept {
        using std::swap;
        swap(first, static_cast<WeatherCondition&>(second));
    }
    IntermediateCondition& IntermediateCondition::operator=(IntermediateCondition rhs) {
        swap(*this, rhs);
        return *this;
    }

   void IntermediateCondition::apply_effects(Team* team) {
    if (const auto* t = team) {
        const int team_bonus = t->getWeatherBonus(Weather_types::INTERMEDIATE);

        Driver_Car pair1 = t->get_driver_car(1);
        pair1.car->apply_race_upgrade(team_bonus);
        if (pair1.driver) {
            const int exp_bonus = pair1.driver->get_performance().experience > 75 ? 15 : 5;
            const int skill = pair1.driver->get_skill(Weather_types::INTERMEDIATE);
            pair1.driver->apply_race_upgrade(skill + exp_bonus);
        }

        Driver_Car pair2 = t->get_driver_car(2);
        pair2.car->apply_race_upgrade(team_bonus);
        if (pair2.driver) {
            const int exp_bonus = pair2.driver->get_performance().experience > 75 ? 15 : 5;
            const int skill = pair2.driver->get_skill(Weather_types::INTERMEDIATE);
            pair2.driver->apply_race_upgrade(skill + exp_bonus);
        }
        
        std::cout << "Intermediate conditions:\n"
                  << "Experienced drivers gaining advantage!\n";
    }
}

    void IntermediateCondition::remove_effects(Team* team) {
    if (const auto* t = team) {
        const int team_bonus = t->getWeatherBonus(Weather_types::INTERMEDIATE);

        Driver_Car pair1 = t->get_driver_car(1);
        pair1.car->remove_race_upgrade(team_bonus);
        if (pair1.driver) {
            const int exp_bonus = pair1.driver->get_performance().experience > 75 ? 15 : 5;
            const int skill = pair1.driver->get_skill(Weather_types::INTERMEDIATE);
            pair1.driver->remove_race_upgrade(skill + exp_bonus);
        }

        Driver_Car pair2 = t->get_driver_car(2);
        pair2.car->remove_race_upgrade(team_bonus);
        if (pair2.driver) {
            const int exp_bonus = pair2.driver->get_performance().experience > 75 ? 15 : 5;
            const int skill = pair2.driver->get_skill(Weather_types::INTERMEDIATE);
            pair2.driver->remove_race_upgrade(skill + exp_bonus);
        }
    }
}
void IntermediateCondition::print_(std::ostream& os) const {
        WeatherCondition::print_(os);
        os << "Intermediate conditions - experience matters\n"
           << "Veteran drivers have an advantage\n";
    }