#ifndef WEATHERCONDITION_H
#define WEATHERCONDITION_H
#include <memory>
#include <string>
#include "Team.h"

class WeatherCondition {
public:
    WeatherCondition() :  lap_time_modifier(0) {}
    WeatherCondition(const WeatherCondition& other) = default;
    WeatherCondition& operator=(const WeatherCondition& other) = default;
    virtual ~WeatherCondition() = default;
    void print_(std::ostream& os) const;
    
    [[nodiscard]] virtual std::unique_ptr<WeatherCondition> clone() const = 0;
    virtual void apply_effects(Team* team) = 0;
    virtual void remove_effects(Team* team) = 0;

    [[nodiscard]] int get_lap_time_modifier() const;
    [[nodiscard]] const std::string& get_name() const;

    friend std::ostream& operator<<(std::ostream& os, const WeatherCondition& obj);

private:
    std::string name;
    int lap_time_modifier;
protected:
    WeatherCondition(std::string name_, const int modifier) : name(std::move(name_)), lap_time_modifier(modifier) {}
    [[nodiscard]] virtual int team_bonus(const Car* car) const = 0;
    [[nodiscard]] virtual int driver_bonus(const Driver* driver) const = 0;
    virtual void do_print_(std::ostream& os) const = 0; 

};

#endif