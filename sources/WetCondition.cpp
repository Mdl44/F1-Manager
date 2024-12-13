#include "WetCondition.h"
#include "Team.h"
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
        
        const int team_bonus = t->getWeatherBonus(Weather_types::WET);
        const int mistake_factor = mistake_dist(gen);
        const int total_team_bonus = team_bonus + mistake_factor;
        
        Driver_Car pair1 = t->get_driver_car(1);
        pair1.car->apply_race_upgrade(total_team_bonus);
        if (pair1.driver) {
            const int driver_mistake = mistake_dist(gen);
            const int driver_skill = pair1.driver->get_skill(Weather_types::WET);
            pair1.driver->apply_race_upgrade(driver_skill + driver_mistake);
        }
        
        Driver_Car pair2 = t->get_driver_car(2);
        pair2.car->apply_race_upgrade(total_team_bonus);
        if (pair2.driver) {
            const int driver_mistake = mistake_dist(gen);
            const int driver_skill = pair2.driver->get_skill(Weather_types::WET);
            pair2.driver->apply_race_upgrade(driver_skill + driver_mistake);
        }
        
        std::cout << "Wet conditions affecting performance:\n"
                 << "Team mistake factor: " << mistake_factor << "\n";
    }
}

void WetCondition::remove_effects(Team* team) {
    if (const auto* t = dynamic_cast<Team*>(team)) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> mistake_dist(-20, 10);
        
        const int team_bonus = t->getWeatherBonus(Weather_types::WET);
        const int mistake_factor = mistake_dist(gen);
        const int total_team_bonus = team_bonus + mistake_factor;
        
        Driver_Car pair1 = t->get_driver_car(1);
        pair1.car->remove_race_upgrade(total_team_bonus);
        if (pair1.driver) {
            const int driver_mistake = mistake_dist(gen);
            const int driver_skill = pair1.driver->get_skill(Weather_types::WET);
            pair1.driver->remove_race_upgrade(driver_skill + driver_mistake);
        }
        
        Driver_Car pair2 = t->get_driver_car(2);
        pair2.car->remove_race_upgrade(total_team_bonus);
        if (pair2.driver) {
            const int driver_mistake = mistake_dist(gen);
            const int driver_skill = pair2.driver->get_skill(Weather_types::WET);
            pair2.driver->remove_race_upgrade(driver_skill + driver_mistake);
        }
    }
}

void WetCondition::print_(std::ostream& os) const {
    WeatherCondition::print_(os);
    os << "Wet racing conditions - unpredictable performance\n"
       << "Warning: High risk of mistakes!\n";
}