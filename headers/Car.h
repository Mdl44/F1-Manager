#ifndef CAR_H
#define CAR_H
#include <iostream>
class Car {
    int aerodynamics;
    int powertrain;
    int durability;
    int chasis;
    int upgrades = 0;
    int downgrades = 0;

    int rating() const;
public:
    void upgrade();
    void downgrade();
    void apply_upgrades();
    void apply_downgrades();
    Car(int aerodynamics, int powertrain, int durability, int chasis);
    Car(const Car& other);
    Car& operator=(const Car& other);
    ~Car();
    [[nodiscard]] int get_rating() const;
    friend std::ostream& operator<<(std::ostream& os, const Car& obj);

};

#endif
