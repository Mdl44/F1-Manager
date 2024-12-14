#ifndef CAR_H
#define CAR_H
#include <iostream>
class Car {
    int aerodynamics;
    int powertrain;
    int durability;
    int chasis;
    [[nodiscard]] int rating() const;
public:
    Car(int aerodynamics, int powertrain, int durability, int chasis);
    Car(const Car& other);
    Car& operator=(const Car& other);
    ~Car();
    friend std::ostream& operator<<(std::ostream& os, const Car& obj);

    void apply_downgrade();
    void apply_upgrade();
    [[nodiscard]] int get_rating() const;
    void apply_race_upgrade(int value);
    void remove_race_upgrade(int value);
};
#endif