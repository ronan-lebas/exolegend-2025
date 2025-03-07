#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "gladiator.h"
#include "config.h"
#include <vector>

class GameState {
    private:
        int radius;
        int map[12][12];
        Gladiator *gladiator;
        std::pair<int, int> currentPosition;
    public:
        GameState(Gladiator *gladiator);
        GameState(Gladiator *gladiator, int radius);
        void setRadius(int radius);
        int getRadius();
        void setCurrentPosition(std::pair<int, int> position);
        std::pair<int, int> getCurrentPosition();
        void update();
        int loss(std::pair<int, int> position);
        std::pair<int, int> searchObjective(void);
};

#endif