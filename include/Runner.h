#ifndef RUNNER_H
#define RUNNER_H

#include "RobotController.h"
#include "gladiator.h"

class Runner {
private:
    RobotController controller;
    Gladiator *gladiator;
public:
    Runner(Gladiator *gladiator);
    void run(); // Main loop or execution function
};

#endif