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
    int rating() const;
    float market_value() const;

public:
    Driver(std::string name, int experience, int race_craft,int awareness, int race_pace, Car* car,Team* team);
    Driver(const Driver& other);
    Driver& operator=(const Driver& other);
    ~Driver();
    friend std::ostream& operator<<(std::ostream& os, const Driver& driver);

    std::string& get_name();
    int get_rating() const;
    float get_market_value() const;
    void set_car(Car* car_set);
    void set_team(Team* team_set);
    Car *get_car() const;
    Team *get_team() const;
};

#endif