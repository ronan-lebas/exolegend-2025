#ifndef ROBOT_CONTROLLER_H
#define ROBOT_CONTROLLER_H

#include "RobotDriver.h"
#include "gladiator.h"
#include <vector>
#include "aStar.h"

struct Path {
    std::vector<std::pair<int, int>> waypoints;
};

class RobotController {
private:
    Gladiator *gladiator;
    RobotDriver driver;
    int currentX, currentY;
    float squareSize;
    std::pair<float, float> caseToCoords(int i, int j);
    Path *currentTargetPath;
public:
    RobotController(Gladiator *gladiator);
    void run();
    void goTo(int i, int j);
    Path straightPath(int i, int j);
    Path pathTo(int i, int j);
    void follow(const Path& path);
};

#endif