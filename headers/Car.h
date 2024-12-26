#ifndef CAR_H
#define CAR_H
#include <iostream>
struct CarPerformance {
    int overall_rating;
    int aerodynamics;
    int powertrain;
    int durability;
    int chasis;
};

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
    [[nodiscard]] CarPerformance get_performance() const;
    void apply_upgrades(int value);
    void apply_downgrades(int value);
};
#endif