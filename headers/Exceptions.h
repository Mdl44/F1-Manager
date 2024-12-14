#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <stdexcept>
#include <string>

class Exceptions : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class ConfigurationFileException : public Exceptions {
public:
    explicit ConfigurationFileException(const std::string& file_path);
};

class InvalidDriverException : public Exceptions {
public:
    explicit InvalidDriverException(const std::string& message);
};

class InvalidTeamException : public Exceptions {
public:
    explicit InvalidTeamException(const std::string& message);
};

class WeatherSimulationException : public Exceptions {
public:
    explicit WeatherSimulationException(const std::string& weather_type);
};

class RaceWeekendException : public Exceptions {
public:
    explicit RaceWeekendException(const std::string& race_name);
};

#endif