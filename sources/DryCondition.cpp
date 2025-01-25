#include "DryCondition.h"

DryCondition::DryCondition() : WeatherCondition("Dry", 0) {}

std::unique_ptr<WeatherCondition> DryCondition::clone() const {
    return std::make_unique<DryCondition>(*this);
}

void DryCondition::apply_effects(Team* team) {
    if (!team) return;
    
    Driver_Car pair1 = team->get_driver_car(1);
    Driver_Car pair2 = team->get_driver_car(2);

    const int car_bonus = team_bonus(pair1.car);
    team_dry_bonuses[team] = car_bonus;
    
    if (pair1.driver) {
        const int driver1_bonus = driver_bonus(pair1.driver);
        driver_dry_bonuses[pair1.driver] = driver1_bonus;
        pair1.driver->apply_upgrades(driver1_bonus);
    }
    if (pair2.driver) {
        const int driver2_bonus = driver_bonus(pair2.driver);
        driver_dry_bonuses[pair2.driver] = driver2_bonus;
        pair2.driver->apply_upgrades(driver2_bonus);
    }

    if (pair1.car) pair1.car->apply_upgrades(car_bonus);
    if (pair2.car) pair2.car->apply_upgrades(car_bonus);
}

void DryCondition::remove_effects(Team* team) {
    if (!team) return;
    
    Driver_Car pair1 = team->get_driver_car(1);
    Driver_Car pair2 = team->get_driver_car(2);

    const int car_bonus = team_dry_bonuses[team];
    
    if (pair1.car) pair1.car->apply_downgrades(car_bonus);
    if (pair2.car) pair2.car->apply_downgrades(car_bonus);
    
    if (pair1.driver) {
        pair1.driver->apply_downgrades(driver_dry_bonuses[pair1.driver]);
        driver_dry_bonuses.erase(pair1.driver);
    }
    if (pair2.driver) {
        pair2.driver->apply_downgrades(driver_dry_bonuses[pair2.driver]);
        driver_dry_bonuses.erase(pair2.driver);
    }

    team_dry_bonuses.erase(team);
}

int DryCondition::team_bonus(const Car* car) const {
    if (!car) return 0;
    const auto perf = car->get_performance();

    const float aero_factor = static_cast<float>(perf.aerodynamics) * 0.35f;
    const float power_factor = static_cast<float>(perf.powertrain) * 0.35f;
    const float tire_factor = static_cast<float>(perf.chasis) * 0.20f;
    const float durability_factor = static_cast<float>(perf.durability) * 0.10f;
    
    return static_cast<int>((aero_factor + power_factor + tire_factor + durability_factor) * 0.15f);
}

int DryCondition::driver_bonus(const Driver* driver) const {
    if (!driver) return 0;
    const auto perf = driver->get_performance();

    const float pace_factor = static_cast<float>(perf.race_pace) * 0.40f;
    const float tire_mgmt = static_cast<float>(perf.race_craft) * 0.30f;
    const float exp_factor = static_cast<float>(perf.experience) * 0.20f;
    const float awareness_factor = static_cast<float>(perf.awareness) * 0.10f;
    
    return static_cast<int>((pace_factor + tire_mgmt + exp_factor + awareness_factor) * 0.12f);
}

void DryCondition::do_print_(std::ostream& os) const {
    os << "Dry racing conditions - standard performance\n"
       << "Optimal conditions for pure speed and car performance\n";
}