#ifndef REGULATIONCHANGES_H
#define REGULATIONCHANGES_H
#include <vector>
#include <memory>

class Team;

class RegulationChanges {
    static int calculate_reduction(int stats);
public:
    static void apply_regulation_changes(const std::vector<std::unique_ptr<Team>>& teams);

};

#endif
