#ifndef TEAM_H
#define TEAM_H
#include "Car.h"
#include "Driver.h"
#include "WeatherTypes.h"
#include <memory>

struct Driver_Car{
    Driver* driver;
    Car* car; 
};

class Team {
    std::string name;
    bool player = false;
    std::unique_ptr<Car> car1;
    std::unique_ptr<Car> car2;
    std::unique_ptr<Driver> driver1;
    std::unique_ptr<Driver> driver2;
    int position;
    int upgrade_points = 0;
    int downgrade_points = 0;
    int dry_bonus;
    int intermediate_bonus;
    int wet_bonus;
    int night_bonus;
public:
    void apply_downgrade();

    void set_control(bool value);
    bool swap(const Driver* const& my_driver, const Driver* const& other_driver, Team& other_team);
    [[nodiscard]] bool is_player_controlled() const;
    Team(std::string name, std::unique_ptr<Car> car1, std::unique_ptr<Car> car2, std::unique_ptr<Driver> driver1, std::unique_ptr<Driver> driver2, int initial_position, int dry_bonus, int intermediate_bonus, int wet_bonus, int night_bonus);
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

    [[nodiscard]] int getWeatherBonus(const Weather_types& weather) const; 
    [[nodiscard]] Driver_Car get_driver_car(int index) const;
};
#endif