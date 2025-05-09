#include "RobotController.h"
#include "aStar.h"
#include "config.h"

RobotController::RobotController(Gladiator *gladiator) : gladiator(gladiator), driver(gladiator), targetReached(false), newPathSet(false)
{
    squareSize = gladiator->maze->getSquareSize();
    currentTargetPath = nullptr;
    targetReached = false;
    newPathSet = false;
    speed = FORWARD_SPEED;
}

void RobotController::reset()
{
    gladiator->log("Call of reset function controller");
    squareSize = gladiator->maze->getSquareSize();
    if (currentTargetPath != nullptr)
    {
        delete currentTargetPath;
    }
    currentTargetPath = nullptr;
}

void RobotController::run()
{
    driver.run();

    if (currentTargetPath != nullptr)
    {
        // If no more waypoints, stop and clean up
        if (driver.isTargetReached() && currentTargetPath->waypoints.empty())
        {
            gladiator->log("Path finished");
            Position pos = gladiator->robot->getData().position;
            gladiator->log("Current position: (%f, %f)", pos.x, pos.y);

            targetReached = true;

            delete currentTargetPath;
            currentTargetPath = nullptr;
            driver.stop();
            return;
        }

        // If target is reached, get next waypoint
        if (driver.isTargetReached() || newPathSet) // Ensure it picks the first waypoint too
        {
            std::pair<int, int> next = currentTargetPath->waypoints.front();
            currentTargetPath->waypoints.erase(currentTargetPath->waypoints.begin());

            gladiator->log("Next waypoint: (%d, %d)", next.first, next.second);

            std::pair<float, float> coords = caseToCoords(next.first, next.second);
            driver.goTo(coords.first, coords.second, speed);
            newPathSet = false;
        }
    }
    else
    {
        driver.stop();
    }
}

void RobotController::goTo(int i, int j, float speed)
{
    Path path = pathTo(i, j);
    for (const auto &point : path.waypoints)
    {
        gladiator->log("Waypoint: (%d, %d)", point.first, point.second);
    }
    newPathSet = true;
    speed = speed;
    follow(path);
}

Path RobotController::pathTo(int i, int j)
{
    return aStar(gladiator->maze->getNearestSquare(), gladiator->maze->getSquare(i, j));
}

Path RobotController::straightPath(int i, int j)
{
    Path path;
    MazeSquare *position = gladiator->maze->getNearestSquare();

    int x = position->i;
    int y = position->j;

    while (x != i || y != j)
    {
        if (abs(i - x) > abs(j - y))
        {
            x += (i > x) ? 1 : -1;
        }
        else
        {
            y += (j > y) ? 1 : -1;
        }
        path.waypoints.emplace_back(x, y);
    }

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
    targetReached = false;
    if (currentTargetPath != nullptr)
    {
        delete currentTargetPath;
    }
    currentTargetPath = new Path(path);
}

std::pair<float, float> RobotController::caseToCoords(int i, int j)
{
    return std::make_pair((float)(i + 0.5) * squareSize, (float)(j + 0.5) * squareSize);
}