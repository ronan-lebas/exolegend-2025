#ifndef ROBOT_DRIVER_H
#define ROBOT_DRIVER_H

#include "gladiator.h"

class RobotDriver {
private:
    Gladiator *gladiator;
    bool targetReached;
    float targetX, targetY;
    float squareSize;
    float speed;
public:
    RobotDriver(Gladiator *gladiator);
    void run();
    void reset();
    bool isTargetReached() { return targetReached; }
    void goTo(float x, float y, float speed);
    void forward(float speed);
    void rotateLeft();
    void rotateRight();
    void forwardLeft();
    void forwardRight();
    void stop();
};

#endif