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
}

void Runner::run()
{
    if (millis() - time4 >= 50)
    {
        // Are we in the Maze ??
        MazeSquare *currentSquare = gladiator->maze->getNearestSquare();
        bool inTheMaze = controller.areWeInTheMaze();
        if(!inTheMaze)
        {
            gladiator->log("We are not in the maze !!");
            controller.returnToMaze();
        } else {
            // gladiator->log("Current position: %d, %d", currentSquare->i, currentSquare->j);
        }
        time4 = millis();
    }

    if (controller.isTargetReached() || controller.hasNoTarget())
    {
        objective = gameState.searchObjective();
        controller.goTo(objective.first, objective.second);
        gladiator->log("Objective set to %d, %d", objective.first, objective.second);
        time1 = millis();
    }

    if (millis() - time2 >= 100)
    {
        // // log if get nearest squre return null
        // if (gladiator->maze->getNearestSquare() == nullptr)
        // {
        //     gladiator->log("Nearest square is null");
        // }
        // else
        // {
        //     gladiator->log("Current position: %d, %d", gladiator->maze->getNearestSquare()->i, gladiator->maze->getNearestSquare()->j);
        // }
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