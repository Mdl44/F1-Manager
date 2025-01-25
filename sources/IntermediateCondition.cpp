#include "IntermediateCondition.h"

IntermediateCondition::IntermediateCondition() : WeatherCondition("Intermediate", 20000) {}

std::unique_ptr<WeatherCondition> IntermediateCondition::clone() const {
    return std::make_unique<IntermediateCondition>(*this);
}

void IntermediateCondition::apply_effects(Team* team) {
    if (!team) return;
    
    Driver_Car pair1 = team->get_driver_car(1);
    Driver_Car pair2 = team->get_driver_car(2);

    const int car_bonus = team_bonus(pair1.car);
    team_temp_impacts[team] = car_bonus;
    
    if (pair1.driver) {
        const int driver1_bonus = driver_bonus(pair1.driver);
        driver_temp_impacts[pair1.driver] = driver1_bonus;
        pair1.driver->apply_upgrades(driver1_bonus);
    }
    if (pair2.driver) {
        const int driver2_bonus = driver_bonus(pair2.driver);
        driver_temp_impacts[pair2.driver] = driver2_bonus;
        pair2.driver->apply_upgrades(driver2_bonus);
    }

    if (pair1.car) pair1.car->apply_upgrades(car_bonus);
    if (pair2.car) pair2.car->apply_upgrades(car_bonus);
}

void IntermediateCondition::remove_effects(Team* team) {
    if (!team) return;
    
    Driver_Car pair1 = team->get_driver_car(1);
    Driver_Car pair2 = team->get_driver_car(2);

    const int car_bonus = team_temp_impacts[team];
    
    if (pair1.car) pair1.car->apply_downgrades(car_bonus);
    if (pair2.car) pair2.car->apply_downgrades(car_bonus);
    
    if (pair1.driver) {
        pair1.driver->apply_downgrades(driver_temp_impacts[pair1.driver]);
        driver_temp_impacts.erase(pair1.driver);
    }
    if (pair2.driver) {
        pair2.driver->apply_downgrades(driver_temp_impacts[pair2.driver]);
        driver_temp_impacts.erase(pair2.driver);
    }

    team_temp_impacts.erase(team);
}

int IntermediateCondition::team_bonus(const Car* car) const {
    if (!car) return 0;
    const auto perf = car->get_performance();

    const float chassis_factor = static_cast<float>(perf.chasis) * 0.40f;
    const float aero_factor = static_cast<float>(perf.aerodynamics) * 0.30f;
    const float durability_factor = static_cast<float>(perf.durability) * 0.30f;
    
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<> grip_variation(-1, 1);
    const int grip_adjustment = grip_variation(gen);

    const float total = (chassis_factor + aero_factor + durability_factor) * 0.15f;
    return std::min(5, static_cast<int>(total) + grip_adjustment);
}

int IntermediateCondition::driver_bonus(const Driver* driver) const {
    if (!driver) return 0;
    const auto perf = driver->get_performance();

    const float adaptability = static_cast<float>(perf.race_craft) * 0.35f;
    const float awareness = static_cast<float>(perf.awareness) * 0.25f;
    const float exp_factor = static_cast<float>(perf.experience) * 0.40f;

    const int exp_bonus = perf.experience > 75 ? 2 : 0;
    
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<> adaptation_var(-1, 1);
    const int adaptation_bonus = adaptation_var(gen);

    const float total = (adaptability + awareness + exp_factor) * 0.12f;
    return std::min(5, static_cast<int>(total) + exp_bonus + adaptation_bonus);
}

void IntermediateCondition::do_print_(std::ostream& os) const {
    os << "Intermediate conditions - experience matters\n"
       << "Balanced setup and adaptability are crucial\n";
}