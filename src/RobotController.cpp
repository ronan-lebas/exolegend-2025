#include "RobotController.h"

RobotController::RobotController(Gladiator *gladiator) : gladiator(gladiator), driver(gladiator), currentX(0), currentY(0) {
    squareSize = gladiator->maze->getSquareSize();
}

void RobotController::run() {
    driver.run();
}

void RobotController::goTo(int i, int j)
{
    Path path = pathTo(i, j);
    follow(path);
}

Path RobotController::pathTo(int i, int j)
{
    Path path;
    // Simple straight-line path for now
    MazeSquare *position = gladiator->maze->getNearestSquare();

    int dx = (i > position->i) ? 1 : -1;
    int dy = (j > position->j) ? 1 : -1;
    for (int x = position->i; x != i; x += dx)
    {
        path.waypoints.emplace_back(x, currentY);
    }
    for (int y = position->j; y != j; y += dy)
    {
        path.waypoints.emplace_back(i, y);
    }
    path.waypoints.emplace_back(i, j);

    // Log the path
    gladiator->log("Path to (%d, %d):", i, j);
    for (const auto &point : path.waypoints)
    {
        gladiator->log("(%d, %d)", point.first, point.second);
    }

    return path;
}

void RobotController::follow(const Path &path)
{
    for (const auto &point : path.waypoints)
    {
        std::pair<int, int> coords = caseToCoords(point.first, point.second);
        if(driver.isTargetReached())
            driver.goTo(coords.first, coords.second);        
    }
}

std::pair<int, int> RobotController::caseToCoords(int i, int j)
{
    return std::make_pair((i + 0.5) * squareSize, (j + 0.5) * squareSize);
}