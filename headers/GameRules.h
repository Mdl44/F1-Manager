#ifndef GAMERULES_H
#define GAMERULES_H

// Tunable game-balance constants, grouped by domain.
struct GameRules {
    struct Driver {
        static constexpr int MIN_STAT = 0;
        static constexpr int MAX_STAT = 100;
        static constexpr int MIN_AGE = 16;
        static constexpr int MAX_AGE = 50;
        static constexpr int RETIREMENT_AGE = 45;
        static constexpr float MARKET_VALUE_BASELINE = 55.0f;
    };

    struct Car {
        static constexpr int MIN_STAT = 0;
        static constexpr int MAX_STAT = 100;
    };

    struct Team {
        static constexpr int DOWNGRADE_POSITION_THRESHOLD = 2;
        static constexpr int AI_AUTO_UPGRADE_THRESHOLD = 5;
        static constexpr float UPGRADE_TO_BUDGET_RATE = 0.5f;
        static constexpr int TOP_TEAM_RATING_THRESHOLD = 85;
    };

    struct CustomTeam {
        static constexpr float INITIAL_BUDGET = 50.0f;
        static constexpr int BASE_CAR_STAT = 55;
    };

    struct Race {
        static constexpr int GRID_POSITION_TIME_PENALTY_MS = 500;
        static constexpr long long RACE_BASE_TIME_MS = 1000000;
        static constexpr int LAP_TIME_FIXED_OVERHEAD_MS = 2000;
        static constexpr int QUALI_RANDOM_VARIATION_MS = 200;
        static constexpr int RACE_RANDOM_VARIATION_MS = 100;
    };

    struct Season {
        static constexpr int POINTS_TABLE[] = {25, 18, 15, 12, 10, 8, 6, 4, 2, 1};
        static constexpr int FASTEST_LAP_BONUS_POINT = 1;
        static constexpr int CHAMPIONSHIP_BONUS_POSITIONS = 3;
    };
};

#endif
