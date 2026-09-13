#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H
#include "GameView.h"

// Today's front end: the same std::cin/std::cout interaction the game
// always had, just moved behind the GameView seam.
class ConsoleView : public GameView {
public:
    void showMessage(const std::string& message) override;
    int askChoice(const std::string& prompt, int min, int max) override;
    std::string askLine(const std::string& prompt) override;
};

#endif
