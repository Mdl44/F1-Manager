#include "ConsoleView.h"
#include <iostream>
#include <limits>
#include <cstdlib>

namespace {
    // Exits cleanly once stdin is closed, instead of retrying forever.
    void exit_if_input_closed() {
        if (std::cin.eof()) {
            std::exit(0);
        }
    }
}

void ConsoleView::showMessage(const std::string& message) {
    std::cout << message;
}

int ConsoleView::askChoice(const std::string& prompt, const int min, const int max) {
    std::cout << prompt;

    int choice;
    if (!(std::cin >> choice)) {
        exit_if_input_closed();
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return min - 1;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice < min || choice > max) {
        return min - 1;
    }
    return choice;
}

std::string ConsoleView::askLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    exit_if_input_closed();
    return line;
}
