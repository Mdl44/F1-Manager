#ifndef CAR_H
#define CAR_H
#include <iostream>
class Team;
class Car {
    int aerodynamics;
    int powertrain;
    int durability;
    int chasis;
    Team* team;

    int rating() const;
public:
    Car(int aerodynamics, int powertrain, int durability, int chasis, Team* team);
    Car(const Car& other);
    Car& operator=(const Car& other);
    ~Car();
    friend std::ostream& operator<<(std::ostream& os, const Car& obj);
    int get_rating() const;
    void set_team(Team* team_set);
};

#endif