#ifndef WEATHERCONDITION_H
#define WEATHERCONDITION_H

#include <memory>
#include <string>

class Team;

class WeatherCondition {
public:
    virtual ~WeatherCondition() = default;
    [[nodiscard]] virtual std::unique_ptr<WeatherCondition> clone() const = 0;
    
    virtual void apply_effects(Team* team) = 0;
    virtual void remove_effects(Team* team) = 0;

    virtual void print(std::ostream& os) const;
    [[nodiscard]] int get_lap_time_modifier() const;
    [[nodiscard]] const std::string& get_name() const;

    WeatherCondition& operator=(const WeatherCondition& other);
    WeatherCondition(WeatherCondition&&) = default;
    WeatherCondition(std::string name, int modifier);

    friend std::ostream& operator<<(std::ostream& os, const WeatherCondition& obj);
    friend void swap(WeatherCondition& first, WeatherCondition& second) noexcept;
    WeatherCondition(const WeatherCondition& other);
    

protected:
    virtual void print_(std::ostream& os) const;
    
private:
    std::string name;
    int lap_time_modifier;
};

#endif