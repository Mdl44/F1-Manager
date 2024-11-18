#ifndef TEAM_H
#define TEAM_H
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
    //bool is_player_controlled() const;
    Team(std::string name, Car* car1, Car* car2, Driver* driver1, Driver* driver2, int initial_position);
    virtual ~Team();
    Team(const Team& other);
    Team& operator=(const Team& other);
    friend std::ostream& operator<<(std::ostream& os, const Team& team);

    std::string get_name();
    [[nodiscard]] Driver *get_driver1() const;
    [[nodiscard]] Driver *get_driver2() const;
    //[[nodiscard]] Car* get_car1() const;
    //[[nodiscard]] Car* get_car2() const;
    //[[nodiscard]] int get_position() const;
};
#endif
