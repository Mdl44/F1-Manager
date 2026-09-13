#ifndef GAMEVIEW_H
#define GAMEVIEW_H
#include <string>

// Abstracts every point where the game talks to or asks something of the
// player, so the simulation can run behind a console today and an SFML
// front end later without touching the game logic itself.
class GameView {
public:
    virtual ~GameView() = default;

    virtual void showMessage(const std::string& message) = 0;
    virtual int askChoice(const std::string& prompt, int min, int max) = 0;
    virtual std::string askLine(const std::string& prompt) = 0;
};

#endif
