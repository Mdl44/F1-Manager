#include "Driver.h"
#include "Car.h"
#include "Team.h"
#include <iostream>

int main() {
    Driver lando("Lando Norris", 81, 90, 84, 93, nullptr, nullptr);
    Driver oscar("Oscar Piastri", 71, 89, 89, 88, nullptr, nullptr);
    Driver lewis("Lewis Hamilton", 99, 97, 96, 95, nullptr, nullptr);
    Driver george("George Russell", 85, 93, 91, 90, nullptr, nullptr);

    Car car1(80, 85, 90, 80, nullptr);
    Car car2(80, 85, 90, 80, nullptr);
    Car mercedes_car1(90, 95, 95, 90, nullptr);
    Car mercedes_car2(90, 95, 95, 90, nullptr);

    Team mclaren("McLaren", &car1, &car2, &lando, &oscar, 2);
    Team mercedes("Mercedes", &mercedes_car1, &mercedes_car2, &lewis, &george, 1);

    std::cout << "Details:\n";
    std::cout << mclaren << "\n";
    std::cout << mercedes << "\n";

    std::cout << "Upgrading " << lando.get_name() << std::endl;
    for (int i = 0; i < 12; i++) {
        lando.upgrade_skill();
    }

    std::cout << "Upgrading " << mercedes.get_name() << std::endl;
    for (int i = 0; i < 12; i++) {
        mercedes_car1.upgrade();
    }

    mercedes.set_control(true);
    std::cout << mercedes.get_name() << " is player controlled\n";

    std::cout << "Driver swap:\n";
    Driver* lando_ptr = &lando;
    Driver* george_ptr = &george;
    if (mclaren.swap(lando_ptr, george_ptr, mercedes)) {
        std::cout << "After swap:\n";
        std::cout << mclaren << "\n";
        std::cout << mercedes << "\n";
    }
    return 0;
}