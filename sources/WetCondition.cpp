#include "WetCondition.h"
#include <iostream>

WetCondition::WetCondition() : WeatherCondition("Wet", 25000) {}

std::unique_ptr<WeatherCondition> WetCondition::clone() const {
    return std::make_unique<WetCondition>(*this);
}

WetCondition::WetCondition(const WetCondition& other) : WeatherCondition(other) {}

WetCondition& WetCondition::operator=(const WetCondition& other) {
    if (this != &other) {
        WeatherCondition::operator=(other);
    }
    return *this;
}


void WetCondition::apply_effects(Team* team) {
    if (const auto* t = dynamic_cast<Team*>(team)) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> mistake_dist(-20, 10);
        
        const int team_bonus = t->get_wet_bonus();
        const int mistake_factor = mistake_dist(gen);
        const int total_team_bonus = team_bonus + mistake_factor;
        
        t->get_car1()->apply_race_upgrade(total_team_bonus);
        t->get_car2()->apply_race_upgrade(total_team_bonus);
        
        if (t->get_driver1()) {
            const int driver_mistake = mistake_dist(gen);
            const int driver_skill = t->get_driver1()->get_wet_skill();
            t->get_driver1()->apply_race_upgrade(driver_skill + driver_mistake);
        }
        if (t->get_driver2()) {
            const int driver_mistake = mistake_dist(gen);
            const int driver_skill = t->get_driver2()->get_wet_skill();
            t->get_driver2()->apply_race_upgrade(driver_skill + driver_mistake);
        }
        
        std::cout << "Wet conditions affecting performance:\n"
                 << "Team mistake factor: " << mistake_factor << "\n";
    }
}

void WetCondition::remove_effects(Team* team) {
    if (const auto* t = dynamic_cast<Team*>(team)) {
        const int team_bonus = t->get_wet_bonus();
        t->get_car1()->remove_race_upgrade(team_bonus);
        t->get_car2()->remove_race_upgrade(team_bonus);

        if (t->get_driver1()) {
            t->get_driver1()->remove_race_upgrade(t->get_driver1()->get_wet_skill());
        }
        if (t->get_driver2()) {
            t->get_driver2()->remove_race_upgrade(t->get_driver2()->get_wet_skill());
        }
    }
}

void WetCondition::print_(std::ostream& os) const {
    WeatherCondition::print_(os);
    os << "Wet racing conditions - unpredictable performance\n"
       << "Warning: High risk of mistakes!\n";
}