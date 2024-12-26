#ifndef DRIVER_H
#define DRIVER_H
#include <string>

struct DriverPerformance {
    int overall_rating;
    float market_value;
    int experience;
    int race_craft;
    int awareness;
    int race_pace;
};

class Driver {
    std::string name;
    int experience;
    int race_craft;
    int awareness;
    int race_pace;
    float value;
    int age;
    [[nodiscard]] int rating() const;
    [[nodiscard]] float market_value() const;

public:
    Driver(std::string name, int experience, int race_craft, int awareness, int race_pace, int age);
    Driver(const Driver& other);
    Driver& operator=(const Driver& other);
    ~Driver();
    friend std::ostream& operator<<(std::ostream& os, const Driver& driver);
    void apply_upgrades(int value_);
    void apply_downgrades(int value_);
    void increase_age();

    [[nodiscard]] const std::string& get_name() const;
    [[nodiscard]]DriverPerformance get_performance() const;
    [[nodiscard]] int get_age() const;
};

#endif