#include "Runner.h"

Runner::Runner(Gladiator *gladiator) : controller(gladiator), gameState(gladiator), gladiator(gladiator)
{
    // Initialization
    time1 = millis();
    time2 = millis();

    objective = std::make_pair(5, 5);
}

void Runner::instructions()
{
    objective = gameState.searchObjective();
    controller.goTo(objective.first, objective.second);
    gladiator->log("Objective set to %d, %d", objective.first, objective.second);

    // Example usage
    // controller.goTo(11, 1);
}

void Runner::run()
{
    if (controller.isTargetReached())
    {
        objective = gameState.searchObjective();
        controller.goTo(objective.first, objective.second);
        gladiator->log("Objective set to %d, %d", objective.first, objective.second);
    }

    if (millis() - time2 >= 100)
    {
        gameState.updateVisited();
        time2 = millis();
    }

    controller.run();
}