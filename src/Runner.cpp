#include "Runner.h"

Runner::Runner(Gladiator *gladiator) : controller(gladiator), gameState(gladiator), gladiator(gladiator) 
{
    // Initialization
    time_at_start = 0;

    time1 = millis();
    time2 = millis();
    time3 = millis();
    time4 = millis();

    currentMazeSize = gladiator->maze->getCurrentMazeSize();

    objective = std::make_pair(5, 5);

    threshold = INITIAL_THRESHOLD;
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
    time_at_start = millis();
    controller.setTimeAtStart(time_at_start);
    gameState.setTimeAtStart(time_at_start);
}

void Runner::run()
{
    if (millis() - time4 >= 50)
    {
        // Are we in the Maze ??
        // MazeSquare *currentSquare = gladiator->maze->getNearestSquare();
        bool inTheMaze = controller.areWeInTheMaze();
        if (!inTheMaze)
        {
            // gladiator->log("We are not in the maze !!");
            controller.returnToMaze();
        }
        else
        {
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
            float expectedPoints1bomb = pointsExpected1bomb(objective.first, objective.second);
            float expectedPoints2bombs = 0;
            if (gladiator->weapon->canDropBombs(2) > 0)
                expectedPoints2bombs = pointsExpected2bombs(objective.first, objective.second);
            float expectedPoints = expectedPoints1bomb > expectedPoints2bombs
                                       ? expectedPoints1bomb
                                       : expectedPoints2bombs;
            // gladiator->log("Expected points: %f/%f", expectedPoints, threshold);
            if (expectedPoints > threshold)
            {
                // gladiator->log("Dropping bomb at %d, %d", objective.first, objective.second);
                if (expectedPoints2bombs > expectedPoints1bomb && gladiator->weapon->canDropBombs(2) > 0)
                {
                    gladiator->weapon->dropBombs(2);
                }
                else
                {
                    gladiator->weapon->dropBombs(1);
                }
                threshold = INITIAL_THRESHOLD;
            }
            threshold -= THRESHOLD_DECAY;
            threshold = threshold < MIN_THRESHOLD ? MIN_THRESHOLD : threshold;
        }
    }

    controller.run();
}

float Runner::pointsExpected1bomb(int i, int j)
{
    // Get the square at i, j
    MazeSquare *square = gladiator->maze->getSquare(i, j);
    float score = 0.0;
    int myTeam = gladiator->robot->getData().teamId;
    MazeSquare *neighbors[5] = {square, square->northSquare, square->southSquare, square->eastSquare, square->westSquare};
    for (int i = 0; i < 5; i++)
    {
        if (neighbors[i] != nullptr)
        {
            if (neighbors[i]->possession == 0)
            {
                score += 1.0;
            }
            else if (neighbors[i]->possession == myTeam)
            {
                score += 0.0;
            }
            else
            {
                score += 2.0;
            }
        }
    }
    score = score / 5.0;
    return score;
}

float Runner::pointsExpected2bombs(int i, int j)
{
    // Get the square at i, j
    MazeSquare *square = gladiator->maze->getSquare(i, j);
    float score = 0.0;
    int myTeam = gladiator->robot->getData().teamId;
    MazeSquare *neighbors[9] = {square, square->northSquare, square->southSquare, square->eastSquare, square->westSquare, nullptr, nullptr, nullptr, nullptr};
    if (neighbors[1] != nullptr)
        neighbors[5] = neighbors[1]->northSquare;
    if (neighbors[2] != nullptr)
        neighbors[6] = neighbors[2]->southSquare;
    if (neighbors[3] != nullptr)
        neighbors[7] = neighbors[3]->eastSquare;
    if (neighbors[4] != nullptr)
        neighbors[8] = neighbors[4]->westSquare;
    for (int i = 0; i < 9; i++)
    {
        if (neighbors[i] != nullptr)
        {
            if (neighbors[i]->possession == 0)
            {
                score += 1.0;
            }
            else if (neighbors[i]->possession == myTeam)
            {
                score += 0.0;
            }
            else
            {
                score += 2.0;
            }
        }
    }
    score = score / 9.0;
    return score;
}