#ifndef ROBOT_CONTROLLER_H
#define ROBOT_CONTROLLER_H

#include "RobotDriver.h"
#include "gladiator.h"
#include <vector>

struct Path {
    std::vector<std::pair<int, int>> waypoints;
};

class RobotController {
private:
    Gladiator *gladiator;
    RobotDriver driver;
    float squareSize;
    std::pair<float, float> caseToCoords(int i, int j);
    Path *currentTargetPath;
    bool targetReached;
    bool newPathSet;
public:
    RobotController(Gladiator *gladiator);
    void run();
    void reset();
    void goTo(int i, int j);
    Path straightPath(int i, int j);
    Path pathTo(int i, int j);
    void follow(const Path& path);
    bool isTargetReached() { return targetReached; }
};

#endif