#ifndef MENU_H
#define MENU_H
#include "GameManager.h"
#include "Player.h"
#include "Season.h"

class Menu {
private:
    static Menu* instance;
    GameManager& manager;
    Player& player; 
    Season& season;
    Team* my_team;

    Menu(GameManager& manager, Player& player, Season& season);

public:
    Menu(const Menu&) = delete;
    Menu& operator=(const Menu&) = delete;

    static void init(GameManager& manager, Player& player, Season& season);
    static Menu& getInstance();
    void run() const;

private:
    static void displayMenu();
    bool handleChoice(size_t& current_race) const;
    [[nodiscard]] bool handleDriver_swap() const; 
    static bool isValidNumber(int& number);
};

#endif
