#include "WetCondition.h"
#include "Team.h"
#include <iostream>
#include "Exceptions.h"

WetCondition::WetCondition() : WeatherCondition("Wet", 25000) {}

std::unique_ptr<WeatherCondition> WetCondition::clone() const {
    return std::make_unique<WetCondition>(*this);
}

WetCondition::WetCondition(const WetCondition& other) : WeatherCondition(other) {}

void swap(WetCondition& first, WetCondition& second) noexcept {
    using std::swap;
    swap(first, static_cast<WeatherCondition&>(second));
}

WetCondition& WetCondition::operator=(WetCondition rhs) {
    swap(*this, rhs);
    return *this;
}


void WetCondition::apply_effects(Team* team) {
    if (!team) {
        throw WeatherSimulationException("Wet - null team pointer");
    }
    
    const int team_bonus = team->getWeatherBonus(Weather_types::WET);
    if (team_bonus < -20 || team_bonus > 20) {
        throw WeatherSimulationException("Wet - invalid team bonus: " + std::to_string(team_bonus));
    }
    const auto* t = team;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> mistake_dist(-20, 10);
        
    const int bonus = t->getWeatherBonus(Weather_types::WET);
    const int mistake_factor = mistake_dist(gen);
    team_mistake_factors[team] = mistake_factor;
    const int total_team_bonus = bonus + mistake_factor;

    Driver_Car pair1 = t->get_driver_car(1);
    pair1.car->apply_race_upgrade(total_team_bonus);
    if (pair1.driver) {
        const int driver_mistake = mistake_dist(gen);
        driver_mistakes[pair1.driver] = driver_mistake;
        const int driver_skill = pair1.driver->get_skill(Weather_types::WET);
        pair1.driver->apply_race_upgrade(driver_skill + driver_mistake);
    }

    Driver_Car pair2 = t->get_driver_car(2);
    pair2.car->apply_race_upgrade(total_team_bonus);
    if (pair2.driver) {
        const int driver_mistake = mistake_dist(gen);
        driver_mistakes[pair2.driver] = driver_mistake;
        const int driver_skill = pair2.driver->get_skill(Weather_types::WET);
        pair2.driver->apply_race_upgrade(driver_skill + driver_mistake);
    }
        
    std::cout << "Wet conditions affecting performance:\n" << "Team mistake factor: " << mistake_factor << "\n";
    }

void WetCondition::remove_effects(Team* team) {
    if (const auto* t = team) {
        const int team_bonus = t->getWeatherBonus(Weather_types::WET);
        const int mistake_factor = team_mistake_factors[team];
        const int total_team_bonus = team_bonus + mistake_factor;
        
        Driver_Car pair1 = t->get_driver_car(1);
        pair1.car->remove_race_upgrade(total_team_bonus);
        if (pair1.driver) {
            const int driver_mistake = driver_mistakes[pair1.driver];
            const int driver_skill = pair1.driver->get_skill(Weather_types::WET);
            pair1.driver->remove_race_upgrade(driver_skill + driver_mistake);
            driver_mistakes.erase(pair1.driver);
        }
        
        Driver_Car pair2 = t->get_driver_car(2);
        pair2.car->remove_race_upgrade(total_team_bonus);
        if (pair2.driver) {
            const int driver_mistake = driver_mistakes[pair2.driver];
            const int driver_skill = pair2.driver->get_skill(Weather_types::WET);
            pair2.driver->remove_race_upgrade(driver_skill + driver_mistake);
            driver_mistakes.erase(pair2.driver);
        }
        
        team_mistake_factors.erase(team);
    }
}

void WetCondition::print_(std::ostream& os) const {
    WeatherCondition::print_(os);
    os << "Wet racing conditions - unpredictable performance\n"
       << "Warning: High risk of mistakes!\n";
}