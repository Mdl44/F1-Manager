#include "Driver.h"
#include "Car.h"
#include <iostream>


int main() {
    Driver lando("Lando Norris", 81, 90, 84, 93);
    Driver oscar("Oscar Piastri", 71, 89, 89, 88);
    std::cout << lando << std::endl;
    std::cout << oscar << std::endl;
    for(int i = 0; i < 10; i++) {
        lando.upgrade_skill();
    }
    oscar.downgrade_skill();
    std::cout << lando << std::endl;
    std::cout << oscar << std::endl;

    Car car1(80,85,90,80);
    Car car2(80,85,90,80);
    std::cout << car1 << std::endl;
    std::cout << car2 << std::endl;
    for(int i = 0; i < 10; i++) {
        car1.upgrade();
    }
    car2.downgrade();
    std::cout << car1 << std::endl;
    std::cout << car2 << std::endl;

    return 0;
}
