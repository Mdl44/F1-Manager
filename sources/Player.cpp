#include "Player.h"
#include <iostream>

Player::Player(Team* my_team) : my_team(my_team) {
    if(my_team) my_team->set_control(true);
}

Player::Player(const Player& other) : my_team(other.my_team) {
    if(my_team) my_team->set_control(true);
}

Player& Player::operator=(const Player& other) {
    if (this != &other) {
        if (my_team) {
            my_team->set_control(false);
        }
        my_team = other.my_team;
        if (my_team) {
            my_team->set_control(true);
        }
    }
    return *this;
}

bool Player::swap_try(const Driver* const& my_driver, const Driver* const& other_driver, Team& other_team) const {
    if (my_team) {
        return my_team->swap(my_driver, other_driver, other_team);
    }
    std::cout << "No team assigned to player.\n";
    return false;
}

Player::~Player() {
    std::cout << "Destroying player.\n";
    if (my_team) {
        my_team->set_control(false);
        my_team = nullptr;
    }
}

void Player::show_data() const {
    if (!my_team) {
        std::cout << "No team assigned to player.\n";
        return;
    }

    std::cout << "\n=== " << my_team->get_name() << " Team Data ===\n";
    std::cout << std::string(50, '=') << "\n";

    std::cout << *my_team;
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

    if (points_to_apply <= 0) {
        std::cout << "You must apply at least 1 upgrade point.\n";
    } else {
        my_team->apply_upgrade_for_player_team(points_to_apply);
        std::cout << "Applied " << points_to_apply << " upgrade points to the team.\n";
        show_data();
    }
}

std::ostream& operator<<(std::ostream& os, const Player& player) {
    if (player.my_team) {
        os << "Player's Team:\n" << *player.my_team;
    } else {
        os << "Player has no team assigned";
    }
    return os;
}