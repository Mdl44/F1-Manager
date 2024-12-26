#include "WindyCondition.h"

WindyCondition::WindyCondition() : WeatherCondition("Windy", 5000) {}

std::unique_ptr<WeatherCondition> WindyCondition::clone() const {
    return std::make_unique<WindyCondition>(*this);
}

void WindyCondition::apply_effects(Team* team) {
    if (!team) return;
    
    Driver_Car pair1 = team->get_driver_car(1);
    Driver_Car pair2 = team->get_driver_car(2);

    const int car_bonus = team_bonus(pair1.car);
    team_wind_bonuses[team] = car_bonus;

    if (pair1.driver) {
        const int driver1_bonus = driver_bonus(pair1.driver);
        driver_wind_bonuses[pair1.driver] = driver1_bonus;
        pair1.driver->apply_upgrades(driver1_bonus);
    }
    if (pair2.driver) {
        const int driver2_bonus = driver_bonus(pair2.driver);
        driver_wind_bonuses[pair2.driver] = driver2_bonus;
        pair2.driver->apply_upgrades(driver2_bonus);
    }

    if (pair1.car) pair1.car->apply_upgrades(car_bonus);
    if (pair2.car) pair2.car->apply_upgrades(car_bonus);
}

void WindyCondition::remove_effects(Team* team) {
    if (!team) return;
    
    Driver_Car pair1 = team->get_driver_car(1);
    Driver_Car pair2 = team->get_driver_car(2);

    const int car_bonus = team_wind_bonuses[team];
    
    if (pair1.car) pair1.car->apply_downgrades(car_bonus);
    if (pair2.car) pair2.car->apply_downgrades(car_bonus);
    
    if (pair1.driver) {
        pair1.driver->apply_downgrades(driver_wind_bonuses[pair1.driver]);
        driver_wind_bonuses.erase(pair1.driver);
    }
    if (pair2.driver) {
        pair2.driver->apply_downgrades(driver_wind_bonuses[pair2.driver]);
        driver_wind_bonuses.erase(pair2.driver);
    }

    team_wind_bonuses.erase(team);
}

int WindyCondition::team_bonus(const Car* car) const {
    if (!car) return 0;
    const auto perf = car->get_performance();

    const float aero_stability = static_cast<float>(perf.aerodynamics) * 0.50f;
    const float chassis_balance = static_cast<float>(perf.chasis) * 0.35f;
    const float durability = static_cast<float>(perf.durability) * 0.15f;
    
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution wind_gust(-2, 1);
    const int gust_effect = wind_gust(gen);

    const int aero_bonus = perf.aerodynamics > 85 ? 1 : 0;

    const float total = (aero_stability + chassis_balance + durability) * 0.12f;
    return std::min(5, static_cast<int>(total) + gust_effect + aero_bonus);
}

int WindyCondition::driver_bonus(const Driver* driver) const {
    if (!driver) return 0;
    const auto perf = driver->get_performance();

    const float awareness = static_cast<float>(perf.awareness) * 0.40f;
    const float control = static_cast<float>(perf.race_craft) * 0.35f;
    const float exp_factor = static_cast<float>(perf.experience) * 0.25f;
    
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<> confidence(-1, 1);
    const int confidence_factor = confidence(gen);

    const int exp_bonus = perf.experience > 80 ? 1 : 0;

    const float total = (awareness + control + exp_factor) * 0.15f;
    return std::min(5, static_cast<int>(total) + confidence_factor + exp_bonus);
}

void WindyCondition::print_(std::ostream& os) const {
    os << "Windy conditions - challenging aerodynamics\n"
       << "Team aerodynamic efficiency matters!\n";
}