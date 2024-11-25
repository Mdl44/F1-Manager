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
    [[nodiscard]] int rating() const;
    [[nodiscard]] float market_value() const;

public:
    Driver(std::string name, int experience, int race_craft, int awareness, int race_pace);
    Driver(const Driver& other);
    Driver& operator=(const Driver& other);
    ~Driver();
    friend std::ostream& operator<<(std::ostream& os, const Driver& driver);

    void apply_downgrade();
    void apply_upgrade();

    std::string& get_name();
    [[nodiscard]] int get_rating() const;
    [[nodiscard]] float get_market_value() const;
};

#endif