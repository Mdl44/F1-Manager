#include "Team.h"

Team::Team(std::string name, Car* car1, Car* car2, Driver* driver1, Driver* driver2, const int initial_position)
    : name(std::move(name)), car1(car1), car2(car2), driver1(driver1), driver2(driver2), position(initial_position) {
    if (driver1) {
        driver1->set_team(this);
        driver1->set_car(car1);
    }
    if (driver2) {
        driver2->set_team(this);
        driver2->set_car(car2);
    }
    if (car1) car1->set_team(this);
    if (car2) car2->set_team(this);
}
Team::~Team() {
    std::cout << "Destructor Team: " << name << std::endl;
    delete driver1;
    delete driver2;
    delete car1;
    delete car2;
}

Team::Team(const Team& other) = default;

Team& Team::operator=(const Team& other) = default;

bool Team::swap(Driver*& my_driver, Driver*& other_driver, Team& other_team) {
    if (my_driver->get_market_value() < other_driver->get_market_value()) {
        std::cout << "Can't swap" << std::endl;
        return false;
    }

    Car* my_team_car = nullptr;
    if (driver1 == my_driver) {
        my_team_car = car1;
    } else if (driver2 == my_driver) {
        my_team_car = car2;
    }

    Car* other_team_car = nullptr;
    if (other_team.driver1 == other_driver) {
        other_team_car = other_team.car1;
    } else if (other_team.driver2 == other_driver) {
        other_team_car = other_team.car2;
    }

    if (!my_team_car || !other_team_car) {
        return false;
    }

    //if (driver1 == my_driver) driver1 = other_driver;
    //else driver2 = other_driver;

    //if (other_team.driver1 == other_driver) other_team.driver1 = my_driver;
    //else other_team.driver2 = my_driver;

    my_driver->set_team(&other_team);
    my_driver->set_car(other_team_car);
    other_driver->set_team(this);
    other_driver->set_car(my_team_car);

    std::swap(my_driver, other_driver);

    std::cout << "Swap completed" << std::endl;
    return true;
}

void Team::set_control(const bool value) {
    player = value;
}

//bool Team::is_player_controlled() const {
   /// return player;
//}

std::string Team::get_name() {
    return name;
}

Driver* Team::get_driver1() const {
    return driver1;
}

Driver* Team::get_driver2() const {
    return driver2;
}

//Car* Team::get_car1() const {
    //return car1;
//}

//Car* Team::get_car2() const {
   // return car2;
//}

//int Team::get_position() const {
 //   return position;
//}

std::ostream& operator<<(std::ostream& os, const Team& team) {
    os << "Team: " << team.name << "\n"
       << "Position: " << team.position << "\n"
       << "First Driver Details:\n"
       << *team.driver1 << "\n"
       << "\nFirst Driver's Car:\n"
       << *team.car1 << "\n"
       << "Second Driver Details:\n"
       << *team.driver2 << "\n"
       << "\nSecond Driver's Car:\n"
       << *team.car2;
    return os;
}

