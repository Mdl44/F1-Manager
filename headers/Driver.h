#ifndef DRIVER_H
#define DRIVER_H
#include <string>

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
    [[nodiscard]] int get_dry_skill() const;
    [[nodiscard]] int get_wet_skill() const;
    [[nodiscard]] int get_intermediate_skill() const;
    void apply_downgrade();
    void apply_upgrade();

    std::string& get_name();
    [[nodiscard]] int get_rating() const;
    [[nodiscard]] float get_market_value() const;
    [[nodiscard]] int get_experience() const;
    void apply_race_upgrade(int value_);
    void remove_race_upgrade(int value_);
};

#endif