#ifndef MENU_H
#define MENU_H
#include "GameManager.h"
#include "Player.h"
#include "Season.h"

class Menu {
  public:
    Menu(GameManager &manager, Player &player, Season &season);
    void run() const;
  private:
    static void displayMenu() ;
    bool handleChoice(size_t& current_race) const;
    [[nodiscard]] bool handleDriver_swap() const;
    static bool isValidNumber(int &number);

    GameManager &manager;
    Player &player;
    Season &season;
    Team* my_team;
};

#endif
