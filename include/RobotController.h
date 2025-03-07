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
    int currentX, currentY;
public:
    RobotController(Gladiator *gladiator);
    void goTo(int i, int j);
    Path pathTo(int i, int j);
    void follow(const Path& path);
};

#endif