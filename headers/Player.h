#ifndef PLAYER_H
#define PLAYER_H
#include "Team.h"

class Player {
    Team* my_team;
public:
    bool swap_try(const Driver *const&my_driver, const Driver *const&other_driver, Team &other_team) const;
    ~Player();
    explicit Player(Team* my_team);
    Player(const Player&);
    Player& operator=(const Player&);
    void show_data() const;
    void upgrades() const;
    friend std::ostream& operator<<(std::ostream& os, const Player& player);
};

#endif