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
    int upgrades = 0;
    int downgrades = 0;

    int rating() const;
public:
    Driver(std::string name, int experience, int race_craft,int awareness, int race_pace);
    Driver(const Driver& other);
    Driver& operator=(const Driver& other);
    ~Driver();
    friend std::ostream& operator<<(std::ostream& os, const Driver& driver);

    void upgrade_skill();
    void downgrade_skill();
    float market_value() const;
    std::string& get_name();
    int get_rating() const;
    float get_market_value() const;
    void apply_upgrade();
    void apply_downgrade();
};

#endif
