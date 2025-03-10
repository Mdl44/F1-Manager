#include "Team.h"
#include <iostream>
#include "Exceptions.h"

Team::Team(const int id,
           std::string name, 
           std::unique_ptr<Car> car1, 
           std::unique_ptr<Car> car2,
           std::unique_ptr<Driver> driver1, 
           std::unique_ptr<Driver> driver2,
           std::unique_ptr<Driver> reserve1,
           std::unique_ptr<Driver> reserve2,
           const int initial_position)
    : team_id(id),
      name(std::move(name)),
      car1(std::move(car1)),
      car2(std::move(car2)),
      driver1(std::move(driver1)),
      driver2(std::move(driver2)),
      reserve1(std::move(reserve1)),
      reserve2(std::move(reserve2)),
      position(initial_position) {}

Driver* Team::get_reserve_driver(const int index) const {
    if (index == 1) return reserve1.get();
    if (index == 2) return reserve2.get();
    return nullptr;
}

void Team::promote_reserve_driver(const int reserve_index, const int driver_index) {
    if (reserve_index < 1 || reserve_index > 2 || driver_index < 1 || driver_index > 2) {
        throw InvalidDriverException("Invalid driver indices for promotion");
    }

    auto& reserve_driver = (reserve_index == 1) ? reserve1 : reserve2;
    auto& main_driver = (driver_index == 1) ? driver1 : driver2;

    if (!reserve_driver) {
        throw InvalidDriverException("No reserve driver available");
    }

    main_driver = std::move(reserve_driver);
}

bool Team::swap_with_reserve(const Driver* const& main_driver, const Driver* const& reserve_driver) {
    std::unique_ptr<Driver>* main_ptr = nullptr;
    std::unique_ptr<Driver>* reserve_ptr = nullptr;

    if (driver1.get() == main_driver) main_ptr = &driver1;
    else if (driver2.get() == main_driver) main_ptr = &driver2;

    if (reserve1.get() == reserve_driver) reserve_ptr = &reserve1;
    else if (reserve2.get() == reserve_driver) reserve_ptr = &reserve2;

    if (!main_ptr || !reserve_ptr) return false;

    *main_ptr = std::move(*reserve_ptr);
    return true;
}

void Team::update_performance_points(const int actual_position) {
    const int diff = actual_position - position;
    /*
    std::cout << "Team " << name << " - Initial pos: " << position 
              << ", Current pos: " << actual_position 
              << ", Diff: " << diff << std::endl;
              */

    if (diff == 0) upgrade_points++;
    else if (diff < 0) upgrade_points += 1;
    else if (diff >= 2) downgrade_points += 1;
}

void Team::apply_upgrade_for_ai_team() {
    if (upgrade_points <= 0) {
        std::cout << "No upgrade points available for " << name << ".\n";
        return;
    }

    std::cout << "\nAI team " << name << " is applying upgrades...\n";

    for (int i = 0; i < upgrade_points; i++) {
        if (car1) car1->apply_upgrades(1);
        if (car2) car2->apply_upgrades(1);
        if (driver1) driver1->apply_upgrades(1);
        if (driver2) driver2->apply_upgrades(1);
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
        if (car1) car1->apply_upgrades(1);
        if (car2) car2->apply_upgrades(1);
        if (driver1) driver1->apply_upgrades(1);
        if (driver2) driver2->apply_upgrades(1);
    }

    upgrade_points -= points;
    std::cout << "Applied " << points << " upgrade points to the team.\n";
}

void Team::apply_downgrade() {
    if (downgrade_points <= 0) return;

    std::cout << "\nApplying performance loss for " << name << "...\n";

    for (int i = 0; i < downgrade_points; i++) {
        if (car1) car1->apply_downgrades(1);
        if (car2) car2->apply_downgrades(1);
        if (driver1) driver1->apply_downgrades(1);
        if (driver2) driver2->apply_downgrades(1);
    }

    downgrade_points = 0;
}

Team::~Team() {
    std::cout << "Destructor Team: " << name << std::endl;
}

Team::Team(const Team& other)
    : team_id(other.team_id),
      name(other.name),
      player(other.player),
      position(other.position),
      upgrade_points(other.upgrade_points),
      downgrade_points(other.downgrade_points),
      budget(other.budget) {
    if (other.car1) car1 = std::make_unique<Car>(*other.car1);
    if (other.car2) car2 = std::make_unique<Car>(*other.car2);
    if (other.driver1) driver1 = std::make_unique<Driver>(*other.driver1);
    if (other.driver2) driver2 = std::make_unique<Driver>(*other.driver2);
    if (other.reserve1) reserve1 = std::make_unique<Driver>(*other.reserve1);
    if (other.reserve2) reserve2 = std::make_unique<Driver>(*other.reserve2);
}

Team& Team::operator=(const Team& other) {
    if (this != &other) {
        team_id = other.team_id;
        name = other.name;
        player = other.player;
        position = other.position;
        upgrade_points = other.upgrade_points;
        downgrade_points = other.downgrade_points;
        budget = other.budget;
        car1 = other.car1 ? std::make_unique<Car>(*other.car1) : nullptr;
        car2 = other.car2 ? std::make_unique<Car>(*other.car2) : nullptr;
        driver1 = other.driver1 ? std::make_unique<Driver>(*other.driver1) : nullptr;
        driver2 = other.driver2 ? std::make_unique<Driver>(*other.driver2) : nullptr;
        reserve1 = other.reserve1 ? std::make_unique<Driver>(*other.reserve1) : nullptr;
        reserve2 = other.reserve2 ? std::make_unique<Driver>(*other.reserve2) : nullptr;
    }
    return *this;
}

bool Team::swap(const Driver* const& my_driver, const Driver* const& other_driver, Team& other_team) {
    if (!my_driver || !other_driver) {
        throw InvalidDriverException("Null driver reference in swap operation");
    }

    float total_value = my_driver->get_performance().market_value + budget;
    if (total_value < other_driver->get_performance().market_value) {
        std::cout << "Can't swap: insufficient combined value (driver + budget)\n";
        std::cout << "Your value: " << total_value << " (driver: " 
                 << my_driver->get_performance().market_value 
                 << " + budget: " << budget << ")\n";
        std::cout << "Target driver value: " << other_driver->get_performance().market_value << "\n";
        return false;
    }

    if (my_driver->get_performance().market_value < other_driver->get_performance().market_value) {
        const float difference = other_driver->get_performance().market_value -
                          my_driver->get_performance().market_value;
        budget -= difference;
        std::cout << "Used " << difference << " from budget for swap.\n";
        std::cout << "Remaining budget: " << budget << "\n";
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

    std::cout << "Swap completed successfully\n";
    return true;
}

Driver_Car Team::get_driver_car(const int index) const {
    if (index == 1) {
        return {driver1.get(), car1.get()};
    } else if (index == 2) {
        return {driver2.get(), car2.get()};
    }
    return {nullptr, nullptr};
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
void Team::convert_points_to_budget() {
    if (upgrade_points > 0) {
        const float conversion = static_cast<float>(upgrade_points) * 0.5f;
        budget += conversion;
        std::cout << name << " converted " << upgrade_points 
                 << " upgrade points to " << conversion << " budget.\n";
        upgrade_points = 0;
    }
}
void Team::check_retirements() {
    if (driver1 && driver1->get_age() >= 45) {
        std::cout << "\n=== DRIVER RETIREMENT ===" << std::endl;
        std::cout << driver1->get_name() << " has retired at age " << driver1->get_age() << std::endl;
        
        if (reserve1) {
            std::cout << "Promoting reserve driver " << reserve1->get_name() << std::endl;
            promote_reserve_driver(1, 1);
        } else if (reserve2) {
            std::cout << "Promoting reserve driver " << reserve2->get_name() << std::endl;
            promote_reserve_driver(2, 1);
        } else {
            std::cout << "No reserve drivers available for promotion!" << std::endl;
        }
    }

    if (driver2 && driver2->get_age() >= 45) {
        std::cout << "\n=== DRIVER RETIREMENT ===" << std::endl;
        std::cout << driver2->get_name() << " has retired at age " << driver2->get_age() << std::endl;
        
        if (reserve1) {
            std::cout << "Promoting reserve driver " << reserve1->get_name() << std::endl;
            promote_reserve_driver(1, 2);
        } else if (reserve2) {
            std::cout << "Promoting reserve driver " << reserve2->get_name() << std::endl;
            promote_reserve_driver(2, 2);
        } else {
            std::cout << "No reserve drivers available for promotion!" << std::endl;
        }
    }
}