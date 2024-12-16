#include "MixedCondition.h"
#include "Team.h"
#include <iostream>

MixedCondition::MixedCondition() : WeatherCondition("Mixed", 10000) {}

std::unique_ptr<WeatherCondition> MixedCondition::clone() const {
    return std::make_unique<MixedCondition>(*this);
}

void MixedCondition::apply_effects(Team* team) {
    if (const auto* t = team) {
        const int dry_bonus = t->getWeatherBonus(Weather_types::DRY);
        const int inter_bonus = t->getWeatherBonus(Weather_types::INTERMEDIATE);
        const int mixed_bonus = (dry_bonus + inter_bonus) / 2;
        team_mixed_bonuses[team] = mixed_bonus;

        Driver_Car pair1 = t->get_driver_car(1);
        Driver_Car pair2 = t->get_driver_car(2);

        if (pair1.car) pair1.car->apply_race_upgrade(mixed_bonus);
        if (pair2.car) pair2.car->apply_race_upgrade(mixed_bonus);

        if (pair1.driver) {
            const int dry_skill = pair1.driver->get_skill(Weather_types::DRY);
            const int inter_skill = pair1.driver->get_skill(Weather_types::INTERMEDIATE);
            const int driver_bonus = (dry_skill + inter_skill) / 2;
            driver_mixed_bonuses[pair1.driver] = driver_bonus;
            pair1.driver->apply_race_upgrade(driver_bonus);
        }

        if (pair2.driver) {
            const int dry_skill = pair2.driver->get_skill(Weather_types::DRY);
            const int inter_skill = pair2.driver->get_skill(Weather_types::INTERMEDIATE);
            const int driver_bonus = (dry_skill + inter_skill) / 2;
            driver_mixed_bonuses[pair2.driver] = driver_bonus;
            pair2.driver->apply_race_upgrade(driver_bonus);
        }

        std::cout << "Mixed conditions:\n"
                  << "Combining dry and intermediate performance characteristics\n"
                  << "Team mixed bonus: " << mixed_bonus << "\n";
    }
}

void MixedCondition::remove_effects(Team* team) {
    if (const auto* t = team) {
        const int mixed_bonus = team_mixed_bonuses[team];

        Driver_Car pair1 = t->get_driver_car(1);
        Driver_Car pair2 = t->get_driver_car(2);

        if (pair1.car) pair1.car->remove_race_upgrade(mixed_bonus);
        if (pair2.car) pair2.car->remove_race_upgrade(mixed_bonus);

        if (pair1.driver) {
            const int driver_bonus = driver_mixed_bonuses[pair1.driver];
            pair1.driver->remove_race_upgrade(driver_bonus);
            driver_mixed_bonuses.erase(pair1.driver);
        }

        if (pair2.driver) {
            const int driver_bonus = driver_mixed_bonuses[pair2.driver];
            pair2.driver->remove_race_upgrade(driver_bonus);
            driver_mixed_bonuses.erase(pair2.driver);
        }

        team_mixed_bonuses.erase(team);
    }
}

void MixedCondition::print_(std::ostream& os) const {
    os << "Mixed racing conditions - balanced performance\n"
       << "Combines characteristics of dry and intermediate conditions\n";
}