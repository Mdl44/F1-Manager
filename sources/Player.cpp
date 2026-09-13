#include "Player.h"
#include <sstream>
#include <limits>

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

bool Player::swap_try(GameView& view, const Driver* const& my_driver, const Driver* const& other_driver, Team& other_team) const {
    if (my_team) {
        auto [success, events] = my_team->swap(my_driver, other_driver, other_team);
        for (const auto& event : events) {
            view.showMessage(event);
        }
        return success;
    }
    view.showMessage("No team assigned to player.\n");
    return false;
}

Player::~Player() {
    if (my_team) {
        my_team->set_control(false);
        my_team = nullptr;
    }
}

void Player::show_data(GameView& view) const {
    if (!my_team) {
        view.showMessage("No team assigned to player.\n");
        return;
    }

    std::ostringstream oss;
    oss << "\n=== " << my_team->get_name() << " Team Data ===\n";
    oss << std::string(50, '=') << "\n";
    oss << *my_team << "\n";
    oss << std::string(50, '=') << "\n";
    view.showMessage(oss.str());
}

void Player::upgrades(GameView& view) const {
    if (my_team->get_upgrade_points() == 0) {
        view.showMessage("You don't have upgrade points available.\n");
        return;
    }

    const int available = my_team->get_upgrade_points();
    const int points_to_apply = view.askChoice(
        "You have " + std::to_string(available) + " upgrade points available.\n"
        "How many upgrade points would you like to apply?\n",
        1, std::numeric_limits<int>::max());

    if (points_to_apply < 1) {
        view.showMessage("You must apply at least 1 upgrade point.\n");
    } else {
        for (const auto& event : my_team->apply_upgrade_for_player_team(points_to_apply)) {
            view.showMessage(event);
        }
        show_data(view);
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
