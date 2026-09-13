#ifndef GAMEVIEW_H
#define GAMEVIEW_H
#include <string>

// Abstracts player interaction so the game logic isn't tied to a console.
class GameView {
public:
    virtual ~GameView() = default;

    virtual void showMessage(const std::string& message) = 0;
    virtual int askChoice(const std::string& prompt, int min, int max) = 0;
    virtual std::string askLine(const std::string& prompt) = 0;
};

#endif
