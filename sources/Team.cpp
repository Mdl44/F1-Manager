#include "Team.h"
#include <iostream>

Team::Team(std::string name, std::unique_ptr<Car> car1, std::unique_ptr<Car> car2, std::unique_ptr<Driver> driver1, std::unique_ptr<Driver> driver2, const int initial_position)
    : name(std::move(name)), car1(std::move(car1)), car2(std::move(car2)), driver1(std::move(driver1)), driver2(std::move(driver2)), position(initial_position) {
}

void Team::update_performance_points(const int actual_position) {
    int diff = actual_position - position;
    if (diff == 0) upgrade_points++;

    else if (diff < 0) upgrade_points += 2;

    else if (diff >= 2) downgrade_points += 1;
}

void Team::apply_upgrade_for_ai_team() {
    if (upgrade_points <= 0) {
        std::cout << "No upgrade points available for " << name << ".\n";
        return;
    }

    std::cout << "\nAI team " << name << " is applying upgrades...\n";

    for (int i = 0; i < upgrade_points; i++) {
        if (car1) car1->apply_upgrade();
        if (car2) car2->apply_upgrade();
        if (driver1) driver1->apply_upgrade();
        if (driver2) driver2->apply_upgrade();
    }

    const int points_used = upgrade_points;
    upgrade_points = 0;
    std::cout << "Applied " << points_used << " upgrade points to the AI team.\n";
}

void Team::apply_upgrade_for_player_team(const int points) {
    if (points <= 0 || points > upgrade_points) {
        std::cout << "Invalid upgrade points for " << name << ".\n";
        return;
    }

    std::cout << "\nPlayer team " << name << " is applying upgrades...\n";

    for (int i = 0; i < points; i++) {
        if (car1) car1->apply_upgrade();
        if (car2) car2->apply_upgrade();
        if (driver1) driver1->apply_upgrade();
        if (driver2) driver2->apply_upgrade();
    }

    upgrade_points -= points;
    std::cout << "Applied " << points << " upgrade points to the team.\n";
}

void Team::apply_downgrade() {
    if (downgrade_points <= 0) return;

    std::cout << "\nApplying performance loss for " << name << "...\n";

    for (int i = 0; i < downgrade_points; i++) {
        if (car1) car1->apply_downgrade();
        if (car2) car2->apply_downgrade();
        if (driver1) driver1->apply_downgrade();
        if (driver2) driver2->apply_downgrade();
    }

    downgrade_points = 0;
}

Team::~Team() {
    std::cout << "Destructor Team: " << name << std::endl;
}

bool Team::swap(const Driver* const& my_driver, const Driver* const& other_driver, Team& other_team) {
    if (my_driver->get_market_value() < other_driver->get_market_value()) {
        std::cout << "Can't swap: market value mismatch" << std::endl;
        return false;
    }

    const Car* my_team_car = nullptr;
    if (driver1.get() == my_driver) {
        my_team_car = car1.get();
    } else if (driver2.get() == my_driver) {
        my_team_car = car2.get();
    }

    const Car* other_team_car = nullptr;
    if (other_team.driver1.get() == other_driver) {
        other_team_car = other_team.car1.get();
    } else if (other_team.driver2.get() == other_driver) {
        other_team_car = other_team.car2.get();
    }

    if (!my_team_car || !other_team_car) {
        std::cout << "Driver or car not found for swap" << std::endl;
        return false;
    }

    if (driver1.get() == my_driver) {
        if (other_team.driver1.get() == other_driver) {
            std::swap(driver1, other_team.driver1);
        } else if (other_team.driver2.get() == other_driver) {
            std::swap(driver1, other_team.driver2);
        }
    } else if (driver2.get() == my_driver) {
        if (other_team.driver1.get() == other_driver) {
            std::swap(driver2, other_team.driver1);
        } else if (other_team.driver2.get() == other_driver) {
            std::swap(driver2, other_team.driver2);
        }
    }

    std::cout << "Swap completed" << std::endl;
    return true;
}

Car* Team::get_car1() const {
    return car1.get();
}
Car* Team::get_car2() const {
    return car2.get();
}
Driver* Team::get_driver1() const {
    return driver1.get();
}
Driver* Team::get_driver2() const {
    return driver2.get();
}

int Team::get_downgrade_points() const {
    return downgrade_points;
}
int Team::get_upgrade_points() const {
    return upgrade_points;
}
void Team::set_control(const bool value) {
    player = value;
}
bool Team::is_player_controlled() const {
    return player;
}
const std::string& Team::get_name() const {
    return name;
}

std::ostream& operator<<(std::ostream& os, const Team& team) {
    os << "Team: " << team.name << "\n"
       << "Position: " << team.position << "\n"
       << "First Driver Details:\n" << *team.driver1 << "\n"
       << "\nFirst Driver's Car:\n" << *team.car1 << "\n"
       << "Second Driver Details:\n" << *team.driver2 << "\n"
       << "\nSecond Driver's Car:\n" << *team.car2;
    return os;
}