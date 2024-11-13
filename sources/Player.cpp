#include "Player.h"

Player::Player(Team *my_team):my_team(my_team) {
    if(my_team) my_team->set_control(true);
}

bool Player::swap_try(Driver*& my_driver, Driver*& other_driver, Team& other_team) const {
    if (my_team) {
        return my_team->swap(my_driver, other_driver, other_team);
    } else {
        std::cout << "No team assigned to player.\n";
        return false;
    }
}
void Player::apply_upgrades(int driver1_upgrades, int driver2_upgrades, int car1_upgrades, int car2_upgrades) const {
    if (my_team) {
        if(my_team->get_driver1() && my_team->get_driver2()) {
            if(my_team->get_driver1() && my_team->get_driver2()) {
                for (int i = 0; i < driver1_upgrades; ++i) my_team->get_driver1()->upgrade_skill();
                for (int i = 0; i < driver2_upgrades; ++i) my_team->get_driver2()->upgrade_skill();
                for (int i = 0; i < car1_upgrades; ++i) my_team->get_car1()->upgrade();
                for (int i = 0; i < car2_upgrades; ++i) my_team->get_car2()->upgrade();

                std::cout << "Upgrades applied for team " << my_team->get_name() << "\n";

            }
        }
    }
}



