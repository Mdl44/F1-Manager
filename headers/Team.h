#ifndef TEAM_H
#define TEAM_H
#include "Car.h"
#include "Driver.h"
#include "WeatherTypes.h"
#include <memory>
#include <unordered_map>
#include "WeatherDetails.h"

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
    std::unique_ptr<Driver> reserve1;
    std::unique_ptr<Driver> reserve2;
    int position;
    int upgrade_points = 0;
    int downgrade_points = 0;
    float budget = 0.0f;
    std::unordered_map<Weather_types, std::unique_ptr<WeatherDetails>> weatherDetails;
public:
    void apply_downgrade();

    void set_control(bool value);
    bool swap(const Driver* const& my_driver, const Driver* const& other_driver, Team& other_team);
    [[nodiscard]] bool is_player_controlled() const;
     Team(std::string name, 
         std::unique_ptr<Car> car1, 
         std::unique_ptr<Car> car2,
         std::unique_ptr<Driver> driver1, 
         std::unique_ptr<Driver> driver2,
         std::unique_ptr<Driver> reserve1,
         std::unique_ptr<Driver> reserve2,
         int initial_position,
         std::unordered_map<Weather_types, std::unique_ptr<WeatherDetails>> weather);
    virtual ~Team();
    Team(const Team& other);
    Team& operator=(const Team& other);
    friend std::ostream& operator<<(std::ostream& os, const Team& team);

    void update_performance_points(int actual_position);
    void apply_upgrade_for_ai_team();
    void apply_upgrade_for_player_team(int points);
    void convert_points_to_budget();

    [[nodiscard]] int get_upgrade_points() const;
    [[nodiscard]] int get_downgrade_points() const;
    [[nodiscard]] const std::string& get_name() const;
    [[nodiscard]] int getWeatherBonus(const Weather_types& weather) const;
    [[nodiscard]] Driver_Car get_driver_car(int index) const;

    [[nodiscard]] Driver* get_reserve_driver(int index) const;
    void promote_reserve_driver(int reserve_index, int driver_index);
    bool swap_with_reserve(const Driver* const& main_driver, const Driver* const& reserve_driver);
};
#endif