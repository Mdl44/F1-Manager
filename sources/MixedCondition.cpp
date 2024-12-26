#include "MixedCondition.h"

MixedCondition::MixedCondition() : WeatherCondition("Mixed", 15000) {}

std::unique_ptr<WeatherCondition> MixedCondition::clone() const {
    return std::make_unique<MixedCondition>(*this);
}

void MixedCondition::apply_effects(Team* team) {
    if (!team) return;
    
    Driver_Car pair1 = team->get_driver_car(1);
    Driver_Car pair2 = team->get_driver_car(2);

    const int car_bonus = team_bonus(pair1.car);
    team_mixed_bonuses[team] = car_bonus;
    
    if (pair1.driver) {
        const int driver1_bonus = driver_bonus(pair1.driver);
        driver_mixed_bonuses[pair1.driver] = driver1_bonus;
        pair1.driver->apply_upgrades(driver1_bonus);
    }
    if (pair2.driver) {
        const int driver2_bonus = driver_bonus(pair2.driver);
        driver_mixed_bonuses[pair2.driver] = driver2_bonus;
        pair2.driver->apply_upgrades(driver2_bonus);
    }

    if (pair1.car) pair1.car->apply_upgrades(car_bonus);
    if (pair2.car) pair2.car->apply_upgrades(car_bonus);
}

void MixedCondition::remove_effects(Team* team) {
    if (!team) return;
    
    Driver_Car pair1 = team->get_driver_car(1);
    Driver_Car pair2 = team->get_driver_car(2);

    const int car_bonus = team_mixed_bonuses[team];
    
    if (pair1.car) pair1.car->apply_downgrades(car_bonus);
    if (pair2.car) pair2.car->apply_downgrades(car_bonus);
    
    if (pair1.driver) {
        pair1.driver->apply_downgrades(driver_mixed_bonuses[pair1.driver]);
        driver_mixed_bonuses.erase(pair1.driver);
    }
    if (pair2.driver) {
        pair2.driver->apply_downgrades(driver_mixed_bonuses[pair2.driver]);
        driver_mixed_bonuses.erase(pair2.driver);
    }

    team_mixed_bonuses.erase(team);
}

int MixedCondition::team_bonus(const Car* car) const {
    if (!car) return 0;
    const auto perf = car->get_performance();

    const float chassis_factor = static_cast<float>(perf.chasis) * 0.40f;
    const float aero_factor = static_cast<float>(perf.aerodynamics) * 0.35f;
    const float durability_factor = static_cast<float>(perf.durability) * 0.25f;
    
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<> condition_var(-2, 2);
    const int adaptation_factor = condition_var(gen);

    const float total = (chassis_factor + aero_factor + durability_factor) * 0.12f;
    return std::min(5, static_cast<int>(total) + adaptation_factor);
}

int MixedCondition::driver_bonus(const Driver* driver) const {
    if (!driver) return 0;
    const auto perf = driver->get_performance();

    const float race_craft = static_cast<float>(perf.race_craft) * 0.35f;
    const float awareness = static_cast<float>(perf.awareness) * 0.35f;
    const float experience = static_cast<float>(perf.experience) * 0.30f;
    
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<> strategy_var(-1, 2);
    const int strategy_bonus = strategy_var(gen);
    const int exp_bonus = perf.experience > 80 ? 1 : 0;

    const float total = (race_craft + awareness + experience) * 0.13f;
    return std::min(5, static_cast<int>(total) + strategy_bonus + exp_bonus);
}

void MixedCondition::print_(std::ostream& os) const {
    os << "Mixed conditions - Adaptability challenge\n"
       << "Strategy and experience are key factors\n";
}