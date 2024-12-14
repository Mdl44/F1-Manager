#ifndef DRIVER_H
#define DRIVER_H
#include <string>
#include "WeatherTypes.h"

struct DriverPerformance {
    int overall_rating;
    float market_value;
    int experience;
};

class Driver {
    std::string name;
    int experience;
    int race_craft;
    int awareness;
    int race_pace;
    float value;
    int age;
    int dry_skill;
    int intermediate_skill;
    int wet_skill;
    [[nodiscard]] int rating() const;
    [[nodiscard]] float market_value() const;

public:
    Driver(std::string name, int experience, int race_craft, int awareness, int race_pace, int age, int dry_skill, int intermediate_skill, int wet_skill);
    Driver(const Driver& other);
    Driver& operator=(const Driver& other);
    ~Driver();
    friend std::ostream& operator<<(std::ostream& os, const Driver& driver);
    void apply_downgrade();
    void apply_upgrade();
    void apply_race_upgrade(int value_);
    void remove_race_upgrade(int value_);

    [[nodiscard]] const std::string& get_name() const;
    [[nodiscard]]DriverPerformance get_performance() const;
    [[nodiscard]] int get_skill(const Weather_types& condition) const;
};

#endif