#ifndef PLAYER_H
#define PLAYER_H
#include "Team.h"

class Player {
    Team* my_team;
public:
    bool swap_try(Driver *&my_driver, Driver *&other_driver, Team &other_team) const;
    explicit Player(Team* my_team);
};

#endif