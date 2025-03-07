#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "gladiator.h"
#include "config.h"
#include <vector>

class GameState {
    private:
        int map[12][12];
        std::pair<int, int> last_visited[NUMBER_OF_CASES_PENALTY];
        Gladiator *gladiator;
    public:
        GameState(Gladiator *gladiator);
        void updateVisited(void);
        float loss(std::pair<int, int> position);
        std::pair<int, int> searchObjective(void);
};

#endif