#ifndef WEATHER_DETAILS_H
#define WEATHER_DETAILS_H
#include <string>
#include <memory>

class WeatherDetails {
protected:
    int bonus = 0;
    [[nodiscard]] virtual std::string getWeatherTypeString() const = 0;

public:
    virtual ~WeatherDetails() = default;
    virtual void readFromFile(const std::string& filepath);
    [[nodiscard]] virtual int getBonus() const { return bonus; }
    virtual void setBonus(const int value) { bonus = value; }
    [[nodiscard]] virtual std::unique_ptr<WeatherDetails> clone() const = 0;
};

class DryWeatherDetails : public WeatherDetails {
public:
    [[nodiscard]] std::unique_ptr<WeatherDetails> clone() const override;
protected:
    [[nodiscard]] std::string getWeatherTypeString() const override;
};

class IntermediateWeatherDetails : public WeatherDetails {
public:
    [[nodiscard]] std::unique_ptr<WeatherDetails> clone() const override;
protected:
    [[nodiscard]] std::string getWeatherTypeString() const override;
};

class WetWeatherDetails : public WeatherDetails {
public:
    [[nodiscard]] std::unique_ptr<WeatherDetails> clone() const override;
protected:
    [[nodiscard]] std::string getWeatherTypeString() const override;
};

class NightWeatherDetails : public WeatherDetails {
public:
    [[nodiscard]] std::unique_ptr<WeatherDetails> clone() const override;
protected:
    [[nodiscard]] std::string getWeatherTypeString() const override;
};

#endif