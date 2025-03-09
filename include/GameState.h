#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "gladiator.h"
#include "config.h"
#include <vector>
#include "dijkstra.h"

class GameState {
    private:
        int map[12][12];
        std::pair<int, int> last_visited[NUMBER_OF_CASES_PENALTY];
        Gladiator *gladiator;
        long time_at_start;
    public:
        GameState(Gladiator *gladiator);
        void reset(void);
        void updateVisited(void);
        void setTimeAtStart(long time) { time_at_start = time; }
        float loss(std::pair<int, int> position, MazeSquare * currentSquare, Position currentPosition, std::unordered_map<MazeSquare*, int> distances);
        std::pair<int, int> searchObjective(void);
};

#endif