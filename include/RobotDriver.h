#ifndef ROBOT_DRIVER_H
#define ROBOT_DRIVER_H

#include "gladiator.h"

class RobotDriver {
private:
    Gladiator *gladiator;
public:
    RobotDriver(Gladiator *gladiator);
    void goTo(double x, double y);
    void rotate(double theta);
};

#endif