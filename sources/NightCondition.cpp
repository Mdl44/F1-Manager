#include "NightCondition.h"
#include "Team.h"
#include <iostream>

NightCondition::NightCondition() : WeatherCondition("Night", 0) {}

std::unique_ptr<WeatherCondition> NightCondition::clone() const {
    return std::make_unique<NightCondition>(*this);
}

void NightCondition::apply_effects(Team* team) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> temp_effect(-15, 15);

    const int team_bonus = team->getWeatherBonus(Weather_types::NIGHT);
    const int temp_impact = temp_effect(gen);
    team_temp_impacts[team] = temp_impact; 
    
    if (const auto* top_team = dynamic_cast<TopTeam*>(team)) {
        const int infra_bonus = TopTeam::getInfrastructureBonus();
        const int total_bonus = team_bonus + temp_impact + infra_bonus;
        
        Driver_Car pair1 = top_team->get_driver_car(1);
        Driver_Car pair2 = top_team->get_driver_car(2);
        
        if (pair1.car) pair1.car->apply_upgrades(total_bonus);
        if (pair2.car) pair2.car->apply_upgrades(total_bonus);

    } else {
        Driver_Car pair1 = team->get_driver_car(1);
        Driver_Car pair2 = team->get_driver_car(2);
        
        int infra_bonus = -5;
        if (pair1.car && pair1.car->get_rating() > 80) {
            infra_bonus = 10;
        }
        
        const int total_bonus = team_bonus + temp_impact + infra_bonus;
        
        if (pair1.car) pair1.car->apply_upgrades(total_bonus);
        if (pair2.car) pair2.car->apply_upgrades(total_bonus);

    }
}

void NightCondition::remove_effects(Team* team) {
    const int team_bonus = team->getWeatherBonus(Weather_types::NIGHT);
    const int temp_impact = team_temp_impacts[team];
    
    if (const auto* top_team = dynamic_cast<TopTeam*>(team)) {
        const int infra_bonus = TopTeam::getInfrastructureBonus();
        const int total_bonus = team_bonus + temp_impact + infra_bonus;
        
        Driver_Car pair1 = top_team->get_driver_car(1);
        Driver_Car pair2 = top_team->get_driver_car(2);
        
        if (pair1.car) pair1.car->apply_downgrades(total_bonus);
        if (pair2.car) pair2.car->apply_downgrades(total_bonus);
    } else {
        Driver_Car pair1 = team->get_driver_car(1);
        Driver_Car pair2 = team->get_driver_car(2);
        
        int infra_bonus = -5;
        if (pair1.car && pair1.car->get_rating() > 80) {
            infra_bonus = 10;
        }
        
        const int total_bonus = team_bonus + temp_impact + infra_bonus;
        
        if (pair1.car) pair1.car->apply_downgrades(total_bonus);
        if (pair2.car) pair2.car->apply_downgrades(total_bonus);
    }
    team_temp_impacts.erase(team); 
}

void NightCondition::print_(std::ostream& os) const {
    os << "Night racing conditions - temperature sensitive\n"
       << "Team infrastructure quality matters!\n";
}