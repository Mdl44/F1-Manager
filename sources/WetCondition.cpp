#include "WetCondition.h"

WetCondition::WetCondition() : WeatherCondition("Wet", 25000) {}

std::unique_ptr<WeatherCondition> WetCondition::clone() const {
    return std::make_unique<WetCondition>(*this);
}

void WetCondition::apply_effects(Team* team) {
    if (!team) return;
    
    Driver_Car pair1 = team->get_driver_car(1);
    Driver_Car pair2 = team->get_driver_car(2);

    const int car_bonus = team_bonus(pair1.car);
    team_mistake_factors[team] = car_bonus;
    
    if (pair1.driver) {
        const int driver1_bonus = driver_bonus(pair1.driver);
        driver_mistakes[pair1.driver] = driver1_bonus;
        pair1.driver->apply_upgrades(driver1_bonus);
    }
    if (pair2.driver) {
        const int driver2_bonus = driver_bonus(pair2.driver);
        driver_mistakes[pair2.driver] = driver2_bonus;
        pair2.driver->apply_upgrades(driver2_bonus);
    }

    if (pair1.car) pair1.car->apply_upgrades(car_bonus);
    if (pair2.car) pair2.car->apply_upgrades(car_bonus);
}

void WetCondition::remove_effects(Team* team) {
    if (!team) return;
    
    Driver_Car pair1 = team->get_driver_car(1);
    Driver_Car pair2 = team->get_driver_car(2);

    const int car_bonus = team_mistake_factors[team];
    
    if (pair1.car) pair1.car->apply_downgrades(car_bonus);
    if (pair2.car) pair2.car->apply_downgrades(car_bonus);
    
    if (pair1.driver) {
        pair1.driver->apply_downgrades(driver_mistakes[pair1.driver]);
        driver_mistakes.erase(pair1.driver);
    }
    if (pair2.driver) {
        pair2.driver->apply_downgrades(driver_mistakes[pair2.driver]);
        driver_mistakes.erase(pair2.driver);
    }

    team_mistake_factors.erase(team);
}

int WetCondition::team_bonus(const Car* car) const {
    if (!car) return 0;
    const auto perf = car->get_performance();

    const float downforce = static_cast<float>(perf.aerodynamics) * 0.45f;
    const float mechanical = static_cast<float>(perf.chasis) * 0.35f;
    const float durability = static_cast<float>(perf.durability) * 0.20f;
    
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<> grip_variation(-2, 1);
    const int grip_factor = grip_variation(gen);

    const float total = (downforce + mechanical + durability) * 0.15f;
    return std::min(5, static_cast<int>(total) + grip_factor);
}

int WetCondition::driver_bonus(const Driver* driver) const {
    if (!driver) return 0;
    const auto perf = driver->get_performance();

    const float awareness = static_cast<float>(perf.awareness) * 0.40f;
    const float craft = static_cast<float>(perf.race_craft) * 0.35f;
    const float exp_factor = static_cast<float>(perf.experience) * 0.25f;
    
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<> mistake_dist(-3, 1);
    int mistake_factor = mistake_dist(gen);
    
    if (perf.experience < 70) mistake_factor -= 1;

    const float total = (awareness + craft + exp_factor) * 0.18f;
    return std::min(5, static_cast<int>(total) + mistake_factor);
}

void WetCondition::do_print_(std::ostream& os) const {
    os << "Wet racing conditions - unpredictable performance\n"
       << "Warning: High risk of mistakes!\n";
}