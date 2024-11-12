#ifndef TEAM_H
#define TEAM_H
#include <string>

#include "Car.h"
#include "Driver.h"

class Team {
    std::string name;
    bool player = false;
    Car* car1;
    Car* car2;
    Driver* driver1;
    Driver* driver2;
    int position;
public:
    void set_control(bool value);
    bool swap(Driver*& my_driver, Driver*& other_driver, Team& other_team);

    Team(std::string name, Car* car1, Car* car2, Driver* driver1, Driver* driver2, int initial_position);
    Team(const Team& other);
    Team& operator=(const Team& other);
    ~Team();
    friend std::ostream& operator<<(std::ostream& os, const Team& team);

    std::string get_name();
    Driver* get_driver1() const;
    Driver* get_driver2() const;
    Car* get_car1() const;
    Car* get_car2() const;
};

#endif
