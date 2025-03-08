#include "GameState.h"

GameState::GameState(Gladiator *gladiator) : gladiator(gladiator)
{
    // Fill last visited queue with NUMBER_OF_CASES_PENALTY -1, -1 pairs
    for (int i = 0; i < NUMBER_OF_CASES_PENALTY; i++)
    {
        last_visited[i] = {-1, -1};
    }
}

void GameState::reset()
{
    // Reset function
    gladiator->log("Call of reset function game state");
    for (int i = 0; i < NUMBER_OF_CASES_PENALTY; i++)
    {
        last_visited[i] = {-1, -1};
    }
}

float GameState::loss(std::pair<int, int> position, MazeSquare *currentSquare, Position currentPosition, std::unordered_map<MazeSquare *, int> distances, Objective objective)
{
    MazeSquare *square = gladiator->maze->getSquare(position.first, position.second);
    float loss = 0;

    loss += WEIGHT_CENTER * std::sqrt(std::pow(currentPosition.x - CENTER_X, 2) + std::pow(currentPosition.y - CENTER_Y, 2));

    float currentSize = gladiator->maze->getCurrentMazeSize();
    int numCases = currentSize/gladiator->maze->getSquareSize();
    int numCasesMargin = numCases / 2;
    loss += WEIGHT_NOGO * (position.first >= MAZE_SIZE - (numCases/2) || position.first < numCases/2 || position.second >= MAZE_SIZE - (numCases/2) || position.second < numCases/2);
    gladiator->log("numCases: %d, numCasesMargin: %d, MAZE_SIZE - (numCases/2): %d", numCases, numCasesMargin, MAZE_SIZE - (numCases/2));

    if (objective == Objective::COLLECT)
    {
        if (distances.find(square) != distances.end())
        {
            loss += WEIGHT_DISTANCE * distances[square];
        }
        else
        {
            loss += WEIGHT_DISTANCE * MAX_DIJKSTRA_DEPTH;
        }

        loss += WEIGHT_ACTIVE_BOMB * square->danger;

        loss += square->coin.value >= 1 ? WEIGHT_UNACTIVE_BOMB : 0;

        int myTeamId = gladiator->robot->getData().teamId;
        RobotList robotList = gladiator->game->getPlayingRobotsId();
        for (int i = 0; i < 4; i++)
        {
            if (robotList.ids[i] == 0 || robotList.ids[i] == gladiator->robot->getData().id) // Skip if robot is not playing or is the current robot
            {
                continue;
            }
            RobotData robot_data = gladiator->game->getOtherRobotData(robotList.ids[i]);
            if (robot_data.teamId != myTeamId)
            {
                loss += WEIGHT_ENEMY * std::sqrt(std::pow(currentPosition.x - robot_data.position.x, 2) + std::pow(currentPosition.y - robot_data.position.y, 2));
            }
            else
            {
                loss += WEIGHT_FRIEND * std::sqrt(std::pow(currentPosition.x - robot_data.position.x, 2) + std::pow(currentPosition.y - robot_data.position.y, 2));
            }
        }

        // Penalize if the square has been visited recently
        float discount_factor = DISCOUNT_FACTOR;
        for (int i = 0; i < NUMBER_OF_CASES_PENALTY; i++)
        {
            if (last_visited[i] == position)
            {
                loss += WEIGHT_STILL * discount_factor;
            }
            discount_factor *= DISCOUNT_FACTOR;
        }
    } else if (objective == Objective::RUN) {
        if (distances.find(square) != distances.end())
        {
            loss += WEIGHT_DISTANCE * distances[square];
        }
        else
        {
            loss += WEIGHT_WALL_RUN;
        }

        loss += WEIGHT_ACTIVE_BOMB_RUN * square->danger;
    }

    return loss;
}

void GameState::updateVisited(void)
{
    MazeSquare *currentSquare = gladiator->maze->getNearestSquare();

    // update last visited array if the position is not already in it
    if (last_visited[0].first != currentSquare->i || last_visited[0].second != currentSquare->j)
    {
        for (int i = NUMBER_OF_CASES_PENALTY - 1; i > 0; i--)
        {
            last_visited[i] = last_visited[i - 1];
        }
        last_visited[0] = {currentSquare->i, currentSquare->j};

        // for (int i = 0; i < NUMBER_OF_CASES_PENALTY; i++)
        // {
        //     gladiator->log("Last visited %d: %d, %d", i, last_visited[i].first, last_visited[i].second);
        // }
    }
}

std::pair<int, int> GameState::searchObjective(Objective objective)
{
    int minLoss = 1000000; // Large number
    std::pair<int, int> bestPosition;

    MazeSquare *currentSquare = gladiator->maze->getNearestSquare();
    Position currentPosition = gladiator->robot->getData().position;
    std::unordered_map<MazeSquare *, int> distances = dijkstra(currentSquare);

    //float losses[MAZE_SIZE][MAZE_SIZE];

    for (int i = 0; i < MAZE_SIZE; i++)
    {
        for (int j = 0; j < MAZE_SIZE; j++)
        {
            int currentLoss = loss({i, j}, currentSquare, currentPosition, distances, objective);
            //losses[i][j] = currentLoss;
            if (currentLoss < minLoss)
            {
                minLoss = currentLoss;
                bestPosition = {i, j};
            }
        }
    }

    // print all square->danger coords value
    // int danger[MAZE_SIZE][MAZE_SIZE];
    // for (int i = 0; i < MAZE_SIZE; i++)
    // {
    //     for (int j = 0; j < MAZE_SIZE; j++)
    //     {
    //         danger[i][j] = gladiator->maze->getSquare(i, j)->danger;
    //     }
    // }
    // for (int i = 0; i < MAZE_SIZE; i++)
    // {
    //     gladiator->log("DANGER %d %d %d %d %d %d %d %d %d %d %d %d", danger[i][0], danger[i][1], danger[i][2], danger[i][3], danger[i][4], danger[i][5], danger[i][6], danger[i][7], danger[i][8], danger[i][9], danger[i][10], danger[i][11]);
    // }

    // Print all losses array
    // for (int i = 0; i < MAZE_SIZE; i++)
    // {
    //     gladiator->log("%.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f", losses[i][0], losses[i][1], losses[i][2], losses[i][3], losses[i][4], losses[i][5], losses[i][6], losses[i][7], losses[i][8], losses[i][9], losses[i][10], losses[i][11]);
    // }

    // gladiator->log("Best position: %d, %d", bestPosition.first, bestPosition.second);
    // gladiator->log("Loss: %d", minLoss);

    return bestPosition;
}