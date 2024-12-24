#ifndef TOPTEAM_H
#define TOPTEAM_H
#include "Team.h"

class TopTeam : public Team {
public:
     TopTeam(std::string name, std::unique_ptr<Car> car1, std::unique_ptr<Car> car2,
            std::unique_ptr<Driver> driver1, std::unique_ptr<Driver> driver2,
            int initial_position, std::unordered_map<Weather_types, std::unique_ptr<WeatherDetails>> weather);

    TopTeam(const TopTeam& other) = default;
    TopTeam& operator=(const TopTeam& other);
    ~TopTeam() override = default;

    [[nodiscard]] static int getInfrastructureBonus();
};

#endif