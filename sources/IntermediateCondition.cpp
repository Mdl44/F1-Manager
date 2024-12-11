#include "IntermediateCondition.h"

    IntermediateCondition::IntermediateCondition(): WeatherCondition("Intermediate", 20000) {}
    
    std::unique_ptr<WeatherCondition> IntermediateCondition::clone() const{
        return std::make_unique<IntermediateCondition>(*this);
    }
    
    IntermediateCondition::IntermediateCondition(const IntermediateCondition& other) : WeatherCondition(other) {}

    IntermediateCondition& IntermediateCondition::operator=(const IntermediateCondition& other) {
        if(this != &other) {
            WeatherCondition::operator=(other);
        }
        return *this;
    }

    void IntermediateCondition::apply_effects(Team* team){
        if (auto* t = dynamic_cast<Team*>(team)) {
            const int team_bonus = t->get_intermediate_bonus();
            t->get_car1()->apply_race_upgrade(team_bonus);
            t->get_car2()->apply_race_upgrade(team_bonus);
            
            if (t->get_driver1()) {
                const int exp_bonus = t->get_driver1()->get_experience() > 75 ? 15 : 5;
                const int skill = t->get_driver1()->get_intermediate_skill();
                t->get_driver1()->apply_race_upgrade(skill + exp_bonus);
            }
            if (t->get_driver2()) {
                const int exp_bonus = t->get_driver2()->get_experience() > 75 ? 15 : 5;
                const int skill = t->get_driver2()->get_intermediate_skill();
                t->get_driver2()->apply_race_upgrade(skill + exp_bonus);
            }
            
            std::cout << "Intermediate conditions:\n"
                     << "Experienced drivers gaining advantage!\n";
        }
    }

    void IntermediateCondition::remove_effects(Team* team){
        if (auto* t = dynamic_cast<Team*>(team)) {
            const int team_bonus = t->get_intermediate_bonus();
            t->get_car1()->remove_race_upgrade(team_bonus);
            t->get_car2()->remove_race_upgrade(team_bonus);
            
            if (t->get_driver1()) {
                t->get_driver1()->remove_race_upgrade(t->get_driver1()->get_intermediate_skill());
            }
            if (t->get_driver2()) {
                t->get_driver2()->remove_race_upgrade(t->get_driver2()->get_intermediate_skill());
            }
        }
    }
void IntermediateCondition::print_(std::ostream& os) const {
        WeatherCondition::print_(os);
        os << "Intermediate conditions - experience matters\n"
           << "Veteran drivers have an advantage\n";
    }