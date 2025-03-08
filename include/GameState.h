#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "gladiator.h"
#include "config.h"
#include <vector>
#include "dijkstra.h"

enum Objective {
    COLLECT,
    RUN,
} typedef Objective;

class GameState {
    private:
        int map[12][12];
        std::pair<int, int> last_visited[NUMBER_OF_CASES_PENALTY];
        Gladiator *gladiator;
    public:
        GameState(Gladiator *gladiator);
        void reset(void);
        void updateVisited(void);
        float loss(std::pair<int, int> position, MazeSquare * currentSquare, Position currentPosition, std::unordered_map<MazeSquare*, int> distances, Objective objective);
        std::pair<int, int> searchObjective(Objective objective);
};

#endif