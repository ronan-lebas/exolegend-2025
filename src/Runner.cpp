#include "Runner.h"

Runner::Runner(Gladiator *gladiator) : controller(gladiator), gameState(gladiator), gladiator(gladiator)
{
    // Initialization
    time1 = millis();
    time2 = millis();
    time3 = millis();

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
    //objective = gameState.searchObjective();
    //controller.goTo(objective.first, objective.second);
    //gladiator->log("Objective set to %d, %d", objective.first, objective.second);

    // Example usage
    // controller.goTo(11, 1);
    objective = choose_simple_objective();
    controller.goTo(objective.first, objective.second);
}

std::pair<int, int> Runner::choose_simple_objective()
{
    std::pair<int, int> currentSquare = std::make_pair(gladiator->maze->getNearestSquare()->i, gladiator->maze->getNearestSquare()->j); 
    std::vector<std::pair<int, int>> objectives;
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            if ((gladiator->maze->getSquare(i, j))->coin.value > 0) {
                objectives.push_back(std::make_pair(i, j));
            }
        }
    }
    if (objectives.size() == 0) {
        return std::make_pair(6, 6);
    }
    if (objectives.size() == 1) {
        return objectives[0];
    }
    else {
        int min = 1000;
        std::pair<int, int> min_pair;
        for (int i = 0; i < objectives.size(); i++) {
            if ( std::sqrt(std::pow(objectives[i].first - currentSquare.first, 2) + std::pow(objectives[i].second - currentSquare.second, 2)) < min ) {
                min_pair = objectives[i];
                min = std::sqrt(std::pow(objectives[i].first - currentSquare.first, 2) + std::pow(objectives[i].second - currentSquare.second, 2));
            }
        }
        return min_pair;
    }
}

void Runner::run()
{
    if (controller.isTargetReached())
    {
        //objective = gameState.searchObjective();
        objective = choose_simple_objective();
        controller.goTo(objective.first, objective.second);
        gladiator->log("Objective set to %d, %d", objective.first, objective.second);
    }

    if (millis() - time2 >= 50)
    {
    //    if(gladiator->maze->getNearestSquare()->danger > 0) {
//            for(int i = 0; i < 10000000; i++){controller.runAway();}
  //      }
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