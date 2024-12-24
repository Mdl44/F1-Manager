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
    [[nodiscard]] int get_rating() const;
    void apply_upgrades(int value);
    void apply_downgrades(int value);
};
#endif