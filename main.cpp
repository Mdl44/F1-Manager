#include "Driver.h"
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

    return 0;
}
