#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H
#include "GameView.h"

// Console front end: reads/writes via std::cin/std::cout.
class ConsoleView : public GameView {
public:
    void showMessage(const std::string& message) override;
    int askChoice(const std::string& prompt, int min, int max) override;
    std::string askLine(const std::string& prompt) override;
};

#endif
