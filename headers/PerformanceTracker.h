#ifndef PERFORMANCETRACKER_H
#define PERFORMANCETRACKER_H
#include <string>
#include <map>

template<typename T>
class PerformanceTracker {
    std::string name;
    std::map<std::string, int> wins;
    std::map<std::string, T> best_values;

public:
    explicit PerformanceTracker(std::string name);
    void add_value(const std::string& entity_name, T value, bool isWin = false);
    [[nodiscard]] std::vector<std::pair<std::string, T>> get_all_highest() const;
    [[nodiscard]] std::vector<std::pair<std::string, int>> get_all_most_wins() const;
    //void reset();
};

#include "../sources/PerformanceTracker.cpp"
#endif