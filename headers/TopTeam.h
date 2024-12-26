#ifndef TOPTEAM_H
#define TOPTEAM_H
#include "Team.h"

class TopTeam : public Team {
public:
    TopTeam(int id,
            std::string name,
            std::unique_ptr<Car> car1,
            std::unique_ptr<Car> car2,
            std::unique_ptr<Driver> driver1,
            std::unique_ptr<Driver> driver2,
            std::unique_ptr<Driver> reserve1,
            std::unique_ptr<Driver> reserve2,
            int initial_position);

    TopTeam(const TopTeam& other) = default;
    TopTeam& operator=(const TopTeam& other);
    ~TopTeam() override = default;

    [[nodiscard]] int getInfrastructureBonus() const;
};

#endif