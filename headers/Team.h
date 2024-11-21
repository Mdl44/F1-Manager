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
    int upgrade_points = 0;
    int downgrade_points = 0;
public:
    void apply_downgrade();

    void set_control(bool value);
    bool swap(Driver*& my_driver, Driver*& other_driver, Team& other_team);
    [[nodiscard]] bool is_player_controlled() const;
    Team(std::string name, Car* car1, Car* car2, Driver* driver1, Driver* driver2, int initial_position);
    virtual ~Team();
    Team(const Team& other);
    Team& operator=(const Team& other);
    friend std::ostream& operator<<(std::ostream& os, const Team& team);

    void update_performance_points(int actual_position);
    void apply_upgrade_for_ai_team();
    void apply_upgrade_for_player_team(int points);

    [[nodiscard]] int get_upgrade_points() const;
    [[nodiscard]] int get_downgrade_points() const;
    [[nodiscard]] const std::string& get_name() const;
    [[nodiscard]] Driver *get_driver1() const;
    [[nodiscard]] Driver *get_driver2() const;
};
#endif
