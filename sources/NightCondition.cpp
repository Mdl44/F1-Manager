#include "NightCondition.h"

NightCondition::NightCondition() : WeatherCondition("Night", 0) {}

std::unique_ptr<WeatherCondition> NightCondition::clone() const {
    return std::make_unique<NightCondition>(*this);
}

void NightCondition::apply_effects(Team* team) {
    if (!team) return;
    
    Driver_Car pair1 = team->get_driver_car(1);
    Driver_Car pair2 = team->get_driver_car(2);

    const int base_car_bonus = team_bonus(pair1.car);
    const auto* top_team = dynamic_cast<TopTeam*>(team);
    const int car_bonus = base_car_bonus + (top_team ? top_team->getInfrastructureBonus() : 0);
    
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

void NightCondition::remove_effects(Team* team) {
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

int NightCondition::team_bonus(const Car* car) const {
    if (!car) return 0;
    const auto perf = car->get_performance();

    const float temp_management = static_cast<float>(perf.chasis)* 0.40f;
    const float power_efficiency = static_cast<float>(perf.powertrain) * 0.35f;
    const float durability = static_cast<float>(perf.durability) * 0.25f;
    
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<> temp_effect(-2, 2);
    const int temp_variation = temp_effect(gen);

    const int infra_bonus = (perf.overall_rating > 80) ? 2 : 0;
    

    const float total = (temp_management + power_efficiency + durability) * 0.12f;
    return std::min(5, static_cast<int>(total) + temp_variation + infra_bonus);
}

int NightCondition::driver_bonus(const Driver* driver) const {
    if (!driver) return 0;
    const auto perf = driver->get_performance();

    const float vision_adapt = static_cast<float>(perf.awareness) * 0.45f;
    const float concentration = static_cast<float>(perf.race_craft) * 0.35f;
    const float night_exp = static_cast<float>(perf.experience) * 0.20f;

    const int exp_bonus = perf.experience > 75 ? 1 : 0;

    const float total = (vision_adapt + concentration + night_exp) * 0.15f;
    return std::min(5, static_cast<int>(total) + exp_bonus);
}

void NightCondition::do_print_(std::ostream& os) const {
    os << "Night racing conditions - temperature sensitive\n"
       << "Team infrastructure and driver awareness are crucial\n";
}