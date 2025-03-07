#include "RobotDriver.h"
#include "gladiator.h"

RobotDriver::RobotDriver(Gladiator *gladiator) : gladiator(gladiator) {}

void RobotDriver::goTo(double x, double y) {
    gladiator->log("Going to (%f, %f)", x, y);
}

void RobotDriver::rotate(double theta) {
    gladiator->log("Rotating by %f", theta);
}
