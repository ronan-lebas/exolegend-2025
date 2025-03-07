#include "GameState.h"

GameState::GameState(Gladiator *gladiator) {
    this->gladiator = gladiator;
    this->radius = 1;
}

GameState::GameState(Gladiator *gladiator, int radius) {
    this->gladiator = gladiator;
    this->radius = radius;
}

void GameState::setRadius(int radius) {
    this->radius = radius;
}

int GameState::getRadius() {
    return this->radius;
}

void GameState::setCurrentPosition(std::pair<int, int> position) {
    this->currentPosition = position;
}

std::pair<int, int> GameState::getCurrentPosition() {
    return this->currentPosition;
}

void GameState::update() {
    return;
}

int GameState::loss(std::pair<int, int> position) {
    MazeSquare * square = gladiator->maze->getSquare(position.first, position.second);
    
    int loss = 0;
    
    loss += WEIGHT_DISTANCE * std::sqrt(std::pow(position.first - currentPosition.first, 2) + std::pow(position.second - currentPosition.second, 2));
    
    loss += WEIGHT_ACTIVE_BOMB * square->danger;
    
    loss += WEIGHT_UNACTIVE_BOMB * square->coin.value;
    
    int myTeamId = gladiator->robot->getData().teamId;
    RobotList robotList = gladiator->game->getPlayingRobotsId();
    for(int i = 0; i < 4; i++) {
        RobotData robot_data = gladiator->game->getOtherRobotData(robotList.ids[i]);
        if (robot_data.teamId != myTeamId) {
            loss += WEIGHT_ENEMY * std::sqrt(std::pow(position.first - robot_data.position.x, 2) + std::pow(position.second - robot_data.position.y, 2));
        }
        else {
            loss += WEIGHT_FRIEND * std::sqrt(std::pow(position.first - robot_data.position.x, 2) + std::pow(position.second - robot_data.position.y, 2));
        }
    }

}

std::pair<int, int> GameState::searchObjective(void) {
    int minLoss = INT_MAX;
    std::pair<int, int> bestPosition;

    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            int currentLoss = loss({i, j});
            if (currentLoss < minLoss) {
                minLoss = currentLoss;
                bestPosition = {i, j};
            }
        }
    }

    return bestPosition;
}