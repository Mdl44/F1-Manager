#include "HotCondition.h"

HotCondition::HotCondition() : WeatherCondition("Hot", 10000) {}

std::unique_ptr<WeatherCondition> HotCondition::clone() const {
    return std::make_unique<HotCondition>(*this);
}

void HotCondition::apply_effects(Team* team) {
    if (!team) return;
    
    Driver_Car pair1 = team->get_driver_car(1);
    Driver_Car pair2 = team->get_driver_car(2);

    const int car_bonus = team_bonus(pair1.car);
    team_heat_impacts[team] = car_bonus;

    if (pair1.car) pair1.car->apply_upgrades(car_bonus);
    if (pair2.car) pair2.car->apply_upgrades(car_bonus);
}

void HotCondition::remove_effects(Team* team) {
    if (!team) return;
    
    Driver_Car pair1 = team->get_driver_car(1);
    Driver_Car pair2 = team->get_driver_car(2);

    const int car_bonus = team_heat_impacts[team];
    
    if (pair1.car) pair1.car->apply_downgrades(car_bonus);
    if (pair2.car) pair2.car->apply_downgrades(car_bonus);

    team_heat_impacts.erase(team);
}

int HotCondition::team_bonus(const Car* car) const {
    if (!car) return 0;
    const auto perf = car->get_performance();

    const float cooling_factor = static_cast<float>(perf.chasis) * 0.40f; 
    const float engine_temp = static_cast<float>(perf.powertrain) * 0.35f;
    const float reliability = static_cast<float>(perf.durability) * 0.25f;
    
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<> heat_variation(-3, 1);
    const int heat_stress = heat_variation(gen);

    const float total = (cooling_factor + engine_temp + reliability) * 0.13f;
    return std::min(5, static_cast<int>(total) + heat_stress);
}
int HotCondition::driver_bonus(const Driver*) const {
    return 0;
}

void HotCondition::do_print_(std::ostream& os) const {
    os << "Hot conditions - cooling systems critical\n"
       << "High temperatures affect car performance\n";
}