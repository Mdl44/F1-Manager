#ifndef DRIVER_H
#define DRIVER_H
#include <string>
class Car;
class Team;

class Driver {
    std::string name;
    int experience;
    int race_craft;
    int awareness;
    int race_pace;
    float value;
    Car* car;
    Team* team;
    [[nodiscard]] int rating() const;
    [[nodiscard]] float market_value() const;

public:
    Driver(std::string name, int experience, int race_craft,int awareness, int race_pace, Car* car,Team* team);
    Driver(const Driver& other);
    Driver& operator=(const Driver& other);
    ~Driver();
    friend std::ostream& operator<<(std::ostream& os, const Driver& driver);

    void apply_downgrade();
    void apply_upgrade();

    std::string& get_name();
    [[nodiscard]] int get_rating() const;
    [[nodiscard]] float get_market_value() const;
    void set_car(Car* car_set);
    void set_team(Team* team_set);
    [[nodiscard]] Car *get_car() const;
    [[nodiscard]] Team *get_team() const;
};

#endif