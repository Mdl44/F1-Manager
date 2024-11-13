#ifndef PLAYER_H
#define PLAYER_H
#include "Team.h"

class Player {
    Team* my_team;
public:
    bool swap_try(Driver *&my_driver, Driver *&other_driver, Team &other_team) const;
    void apply_upgrades(int driver1_upgrades, int driver2_upgrades, int car1_upgrades, int car2_upgrades) const;
    Player(Team* my_team);
};


#endif
