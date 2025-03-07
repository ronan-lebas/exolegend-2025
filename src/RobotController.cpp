#include "RobotController.h"
#include <cmath>

RobotController::RobotController(Gladiator *gladiator) : gladiator(gladiator), driver(gladiator), currentX(0), currentY(0) {}

void RobotController::goTo(int i, int j)
{
    Path path = pathTo(i, j);
    follow(path);
}

Path RobotController::pathTo(int i, int j)
{
    Path path;
    // Simple straight-line path for now
    int dx = (i > currentX) ? 1 : -1;
    int dy = (j > currentY) ? 1 : -1;
    for (int x = currentX; x != i; x += dx)
    {
        path.waypoints.emplace_back(x, currentY);
    }
    for (int y = currentY; y != j; y += dy)
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
        driver.goTo(point.first, point.second);
    }
    currentX = path.waypoints.back().first;
    currentY = path.waypoints.back().second;
}