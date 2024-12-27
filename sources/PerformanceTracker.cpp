#include <algorithm>

template<typename T>
PerformanceTracker<T>::PerformanceTracker(std::string name) : name(std::move(name)) {}

template<typename T>
void PerformanceTracker<T>::add_value(const std::string& entity_name, T value, const bool isWin) {
    if (isWin) {
        wins[entity_name]++;
    }
    
    if (!best_values.contains(entity_name)) {
        best_values[entity_name] = value;
    } else {
        best_values[entity_name] = std::max(best_values[entity_name], value);
    }
}

template<typename T>
std::vector<std::pair<std::string, T>> PerformanceTracker<T>::get_all_highest() const {
    if (best_values.empty()) return {};
    
    T max_value = std::max_element(best_values.begin(), best_values.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; })->second;
        
    std::vector<std::pair<std::string, T>> result;
    for (const auto& [entity_name, value] : best_values) {
        if (value == max_value) {
            result.emplace_back(entity_name, value);
        }
    }
    return result;
}

template<typename T>
std::vector<std::pair<std::string, int>> PerformanceTracker<T>::get_all_most_wins() const {
    if (wins.empty()) return {};

    const int max_wins = std::max_element(wins.begin(), wins.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; })->second;
        
    std::vector<std::pair<std::string, int>> result;
    for (const auto& [entity_name, count] : wins) {
        if (count == max_wins) {
            result.emplace_back(entity_name, count);
        }
    }
    return result;
}

template<typename T>
void PerformanceTracker<T>::reset() {
    wins.clear();
    best_values.clear();
}