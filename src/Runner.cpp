#include "Runner.h"

Runner::Runner(Gladiator *gladiator) : controller(gladiator), gameState(gladiator), gladiator(gladiator)
{
    // Initialization
    time1 = millis();
    time2 = millis();
    time3 = millis();
    time4 = millis();

    currentMazeSize = gladiator->maze->getCurrentMazeSize();

    objective = std::make_pair(5, 5);
}

void Runner::reset()
{
    // Reset function
    // Initialization of all your variables before the start of a match
    gladiator->log("Call of reset function runner");
    controller.reset();
    gameState.reset();
}

void Runner::instructions()
{
    gladiator->control->setWheelPidCoefs(WheelAxis::LEFT, 0.4, 0.8, 0.);
    gladiator->control->setWheelPidCoefs(WheelAxis::RIGHT, 0.4, 0.8, 0.);
    objective = gameState.searchObjective();
    controller.goTo(objective.first, objective.second);
    gladiator->log("Objective set to %d, %d", objective.first, objective.second);

    // Example usage
    // controller.goTo(11, 1);
}

void Runner::run()
{
    if (millis() - time4 >= 50)
    {
        time4 = millis();
        if (currentMazeSize != gladiator->maze->getCurrentMazeSize())
        {
            gladiator->log("Maze size changed");
            objective = gameState.searchObjective();
            controller.goTo(objective.first, objective.second);
            currentMazeSize = gladiator->maze->getCurrentMazeSize();
        }
    }

    if (controller.isTargetReached())
    {
        objective = gameState.searchObjective();
        controller.goTo(objective.first, objective.second);
        gladiator->log("Objective set to %d, %d", objective.first, objective.second);
        time1 = millis();
    }

    if (millis() - time2 >= 100)
    {
        gameState.updateVisited();
        time2 = millis();
    }

    if (millis() - time3 >= 100)
    {
        if (gladiator->weapon->canDropBombs(1) > 0)
        {
            gladiator->weapon->dropBombs(1);
        }
    }

    controller.run();
}