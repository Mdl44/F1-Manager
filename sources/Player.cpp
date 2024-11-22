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
void Player::show_data() const {
    if (!my_team) {
        std::cout << "No team assigned to player.\n";
        return;
    }

    std::cout << "\n=== " << my_team->get_name() << " Team Data ===\n";
    std::cout << std::string(50, '=') << "\n";

    std::cout << "First Driver:\n";
    if (const Driver* driver1 = my_team->get_driver1()) {
        std::cout << *driver1 << "\n";
        if (const Car* car = driver1->get_car()) {
            std::cout << "Car:\n" << *car << "\n";
        }
    }

    std::cout << std::string(30, '-') << "\n";

    std::cout << "Second Driver:\n";
    if (const Driver* driver2 = my_team->get_driver2()) {
        std::cout << *driver2 << "\n";
        if (const Car* car = driver2->get_car()) {
            std::cout << "Car:\n" << *car << "\n";
        }
    }

    std::cout << std::string(50, '=') << "\n";
}



void Player::upgrades() const {
    if (my_team->get_upgrade_points() == 0) {
        std::cout << "You don't have upgrade points available.\n";
        return;
    }

    int points_to_apply;
    std::cout << "You have " << my_team->get_upgrade_points() << " upgrade points available.\n";
    std::cout << "How many upgrade points would you like to apply?\n";
    std::cin >> points_to_apply;

    if (points_to_apply <= 0)  {
        std::cout << "You must apply at least 1 upgrade point.\n";
    } else {
        my_team->apply_upgrade_for_player_team(points_to_apply);
        std::cout << "Applied " << points_to_apply << " upgrade points to the team.\n";
        show_data();
    }
}