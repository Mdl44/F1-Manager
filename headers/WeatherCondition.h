#ifndef WEATHERCONDITION_H
#define WEATHERCONDITION_H

#include <memory>
#include <string>

class Team;

class WeatherCondition {
public:
    WeatherCondition() : name(""), lap_time_modifier(0) {}
    WeatherCondition(const WeatherCondition& other) = default;
    WeatherCondition& operator=(const WeatherCondition& other) = default;
    virtual ~WeatherCondition() = default;
    
    [[nodiscard]] virtual std::unique_ptr<WeatherCondition> clone() const = 0;
    virtual void apply_effects(Team* team) = 0;
    virtual void remove_effects(Team* team) = 0;

    [[nodiscard]] int get_lap_time_modifier() const;
    [[nodiscard]] const std::string& get_name() const;

    friend std::ostream& operator<<(std::ostream& os, const WeatherCondition& obj);

private:
    std::string name;
    int lap_time_modifier;
    virtual void print_(std::ostream& os) const = 0;
protected:
    WeatherCondition(std::string name_, int modifier) : name(std::move(name_)), lap_time_modifier(modifier) {}

};

#endif