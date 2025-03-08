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

float GameState::loss(std::pair<int, int> position, MazeSquare * currentSquare, Position currentPosition, std::unordered_map<MazeSquare *, int> distances)
{
    MazeSquare *square = gladiator->maze->getSquare(position.first, position.second);
    float squareSize = gladiator->maze->getSquareSize();

    float loss = 0;

    loss += WEIGHT_CENTER * std::sqrt(std::pow((position.first + 0.5) * squareSize - CENTER_X, 2) + std::pow((position.second + 0.5) * squareSize - CENTER_Y, 2));

    float currentSize = gladiator->maze->getCurrentMazeSize();
    int numCases = currentSize/gladiator->maze->getSquareSize();
    int numCasesMargin = (MAZE_SIZE - numCases)/2 + 1;
    loss += WEIGHT_NOGO * 
         (position.first >= (MAZE_SIZE - numCasesMargin) 
         || position.first < numCasesMargin
         || position.second >= MAZE_SIZE - numCasesMargin 
         || position.second < numCasesMargin);
    //gladiator->log("numCases: %d, numCasesMargin: %d, MAZE_SIZE - (numCases/2): %d", numCases, numCasesMargin, MAZE_SIZE - (numCases/2));

    if (distances.find(square) != distances.end()) {
        loss += WEIGHT_DISTANCE * ((float)distances[square])/MAX_DIJKSTRA_DEPTH;
    } else {
        loss += WEIGHT_DISTANCE;
    }

    loss += WEIGHT_ACTIVE_BOMB * square->danger;
    if(square->danger > 0) {
        gladiator->log("Danger: %d in %d, %d", square->danger, position.first, position.second);
    }

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

    return loss;
}

void GameState::updateVisited(void)
{
    MazeSquare *currentSquare = gladiator->maze->getNearestSquare();
    if (currentSquare == nullptr)
    {
        gladiator->log("Current square is null");
        return;
    }

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

std::pair<int, int> GameState::searchObjective(void)
{
    int minLoss = 1000000; // Large number
    std::pair<int, int> bestPosition;

    MazeSquare *currentSquare = gladiator->maze->getNearestSquare();
    if (currentSquare == nullptr)
    {
        gladiator->log("Current square is null = impossible to search objective");
        return {6, 6};
    }

    Position currentPosition = gladiator->robot->getData().position;
    std::unordered_map<MazeSquare *, int> distances = dijkstra(currentSquare);

    float losses[MAZE_SIZE][MAZE_SIZE];

    for (int i = 0; i < MAZE_SIZE; i++)
    {
        for (int j = 0; j < MAZE_SIZE; j++)
        {
            float currentLoss = loss({i, j}, currentSquare, currentPosition, distances);
            losses[i][j] = currentLoss;
            if (currentLoss < minLoss)
            {
                minLoss = currentLoss;
                bestPosition = {i, j};
            }
        }
    }

    // Print all losses array
    for 
    (int i = 0; i < MAZE_SIZE; i++)
    {
        gladiator->log("%.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f %.1f", losses[i][0], losses[i][1], losses[i][2], losses[i][3], losses[i][4], losses[i][5], losses[i][6], losses[i][7], losses[i][8], losses[i][9], losses[i][10], losses[i][11]);
    }

    gladiator->log("Best position: %d, %d", bestPosition.first, bestPosition.second);
    gladiator->log("Loss: %d", minLoss);

    return bestPosition;
}