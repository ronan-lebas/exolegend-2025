#ifndef RUNNER_H
#define RUNNER_H

#include "RobotController.h"
#include "GameState.h"
#include "gladiator.h"

class Runner {
private:
    RobotController controller;
    GameState gameState;
    Gladiator *gladiator;
    std::pair<int, int> objective;
    long time1;
    long time2;
    long time3;
    long time4;
    float currentMazeSize;
    float threshold;
public:
    Runner(Gladiator *gladiator);
    void instructions(); // Instructions for the robot
    void run(); // Main loop or execution function
    void reset(); // Reset function
    float pointsExpected(int i, int j); // Points expected to be scored
};

#endif