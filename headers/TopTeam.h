#ifndef TOPTEAM_H
#define TOPTEAM_H
#include "Team.h"

class TopTeam : public Team {
public:
    TopTeam(const std::string& name, std::unique_ptr<Car> car1, std::unique_ptr<Car> car2,std::unique_ptr<Driver> driver1, std::unique_ptr<Driver> driver2,int initial_position,
            int dry_bonus, int intermediate_bonus, int wet_bonus, int night_bonus);

    TopTeam(const TopTeam& other);
    TopTeam& operator=(const TopTeam& other);
    ~TopTeam() override = default;

    [[nodiscard]] static int getInfrastructureBonus();
};

#endif