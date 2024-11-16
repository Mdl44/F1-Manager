#include "Player.h"

Player::Player(Team *my_team):my_team(my_team) {
    if(my_team) my_team->set_control(true);
}

bool Player::swap_try(Driver*& my_driver, Driver*& other_driver, Team& other_team) const {
    if (my_team) {
        return my_team->swap(my_driver, other_driver, other_team);
    }
    std::cout << "No team assigned to player.\n";
    return false;
}
