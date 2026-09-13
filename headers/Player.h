#ifndef PLAYER_H
#define PLAYER_H
#include "Team.h"
#include "GameView.h"

class Player {
    Team* my_team;
public:
    bool swap_try(GameView& view, const Driver *const&my_driver, const Driver *const&other_driver, Team &other_team) const;
    ~Player();
    explicit Player(Team* my_team);
    Player(const Player&);
    Player& operator=(const Player&);
    void show_data(GameView& view) const;
    void upgrades(GameView& view) const;
    friend std::ostream& operator<<(std::ostream& os, const Player& player);
};

#endif