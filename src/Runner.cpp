#include "Runner.h"

Runner::Runner(Gladiator *gladiator) : controller(gladiator), gameState(gladiator), gladiator(gladiator)
{
    // Initialization
    time1 = millis();
    time2 = millis();
    time3 = millis();
    time4 = millis();

    objective = std::make_pair(5, 5);
}

void Runner::reset()
{
    // Reset function
    // Initialization of all your variables before the start of a match
    gladiator->log("Call of reset function runner");
    controller.reset();
    gameState.reset();
    instructions();
}

void Runner::instructions()
{
    gladiator->control->setWheelPidCoefs(WheelAxis::LEFT, 0.4, 0.8, 0.);
    gladiator->control->setWheelPidCoefs(WheelAxis::RIGHT, 0.4, 0.8, 0.);
    objective = gameState.searchObjective(Objective::COLLECT);
    controller.goTo(objective.first, objective.second, FORWARD_SPEED);
    gladiator->log("Objective set to %d, %d", objective.first, objective.second);
}

void Runner::run()
{
    if (controller.isTargetReached() || millis() - time1 >= 4000)
    {
        objective = gameState.searchObjective(Objective::COLLECT);
        controller.goTo(objective.first, objective.second, FORWARD_SPEED);
        gladiator->log("Objective set to %d, %d", objective.first, objective.second);
        time1 = millis();
    }

    if (millis() - time2 >= 400)
    {
        gameState.updateVisited();
        time2 = millis();
    }

    if (millis() - time3 >= 500)
    {
        if (gladiator->weapon->canDropBombs(1))
        {
            gladiator->weapon->dropBombs(1);
            objective = gameState.searchObjective(Objective::RUN);
            controller.goTo(objective.first, objective.second, RUN_AWAY_SPEED);
            gladiator->log("Objective set to %d, %d", objective.first, objective.second);
            time1 = millis();
        }
        time3 = millis();
    }

    if (millis() - time4 >= 20)
    {
        controller.run();
        time4 = millis();
    }
}