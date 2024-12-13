#include "NightCondition.h"
#include "Team.h"
#include <iostream>

NightCondition::NightCondition() : WeatherCondition("Night", 0) {}

std::unique_ptr<WeatherCondition> NightCondition::clone() const {
    return std::make_unique<NightCondition>(*this);
}

NightCondition::NightCondition(const NightCondition& other) : WeatherCondition(other) {}

NightCondition& NightCondition::operator=(const NightCondition& other) {
    if (this != &other) {
        WeatherCondition::operator=(other);
    }
    return *this;
}

void NightCondition::apply_effects(Team* team) {
    if (const auto* t = dynamic_cast<Team*>(team)) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> temp_effect(-15, 15);

        const int team_bonus = t->getWeatherBonus(Weather_types::NIGHT);
        const int temp_impact = temp_effect(gen);

        Driver_Car pair1 = t->get_driver_car(1);
        const int infra_bonus = pair1.car->get_rating() > 80 ? 10 : -5;
        const int total_bonus = team_bonus + temp_impact + infra_bonus;
        
        pair1.car->apply_race_upgrade(total_bonus);
        Driver_Car pair2 = t->get_driver_car(2);
        pair2.car->apply_race_upgrade(total_bonus);
        
        std::cout << "Night race conditions:\n"
                 << "Temperature impact: " << (temp_impact > 0 ? "Warm" : "Cold")
                 << " (" << temp_impact << ")\n"
                 << "Infrastructure bonus: " << infra_bonus << "\n";
    }
}

void NightCondition::remove_effects(Team* team) {
    if (const auto* t = dynamic_cast<Team*>(team)) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> temp_effect(-15, 15);

        const int team_bonus = t->getWeatherBonus(Weather_types::NIGHT);
        const int temp_impact = temp_effect(gen);
        
        Driver_Car pair1 = t->get_driver_car(1);
        const int infra_bonus = pair1.car->get_rating() > 80 ? 10 : -5;
        const int total_bonus = team_bonus + temp_impact + infra_bonus;

        pair1.car->remove_race_upgrade(total_bonus);
        Driver_Car pair2 = t->get_driver_car(2);
        pair2.car->remove_race_upgrade(total_bonus);
    }
}

void NightCondition::print_(std::ostream& os) const {
    WeatherCondition::print_(os);
    os << "Night racing conditions - temperature sensitive\n"
       << "Team infrastructure quality matters!\n";
}