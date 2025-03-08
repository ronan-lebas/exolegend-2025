#include "RobotController.h"
#include "aStar.h"

RobotController::RobotController(Gladiator *gladiator) : gladiator(gladiator), driver(gladiator), targetReached(false), newPathSet(false)
{
    squareSize = gladiator->maze->getSquareSize();
    currentTargetPath = nullptr;
    targetReached = false;
    newPathSet = false;
    returnToMazeMode = false;
}

void RobotController::reset()
{
    gladiator->log("Call of reset function controller");
    squareSize = gladiator->maze->getSquareSize();
    currentTargetPath = nullptr;
}

bool RobotController::areWeInTheMaze()
{
    float currentSize = gladiator->maze->getCurrentMazeSize();
    int numCases = currentSize / gladiator->maze->getSquareSize();
    int numCasesMargin = (MAZE_SIZE - numCases) / 2;
    MazeSquare *currentSquare = gladiator->maze->getNearestSquare();

    return !(currentSquare->i >= (MAZE_SIZE - numCasesMargin) || currentSquare->j < numCasesMargin || currentSquare->i >= MAZE_SIZE - numCasesMargin || currentSquare->j < numCasesMargin);
}

void RobotController::run()
{
    driver.run();

    if (returnToMazeMode)
    {
        if (areWeInTheMaze())
        {
            returnToMazeMode = false;
        }
        else
        {
            //gladiator->log("Returning to maze !! MBAPPE SPEED !!");
            driver.goToMax(CENTER_X, CENTER_Y);
            return;
        }
    }

    if (currentTargetPath != nullptr)
    {
        // print size of the path
        // gladiator->log("Path size: %lu", currentTargetPath->waypoints.size());
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
            if (currentTargetPath->waypoints.size() >= 2)
            {
                // gladiator->log("Recomputing the path");
                // We recompute the path at this point
                Path newPath = pathTo(currentTargetPath->waypoints.back().first, currentTargetPath->waypoints.back().second);
                // Erase first element of the path
                newPath.waypoints.erase(newPath.waypoints.begin());
                // If empty, we are already at the target
                if (newPath.waypoints.empty())
                {
                    return;
                }
                delete currentTargetPath;
                currentTargetPath = new Path(newPath);
            }

            std::pair<int, int> next = currentTargetPath->waypoints.front();
            currentTargetPath->waypoints.erase(currentTargetPath->waypoints.begin());

            gladiator->log("Next waypoint: (%d, %d)", next.first, next.second);

            std::pair<float, float> coords = caseToCoords(next.first, next.second);
            driver.goTo(coords.first, coords.second);
            newPathSet = false;
        }
    }
    else
    {
        driver.stop();
    }
}

void RobotController::goTo(int i, int j)
{
    Path path = pathTo(i, j);
    gladiator->log("Path to (%d, %d) computed :", i, j);
    // print size of path
    gladiator->log("Path size: %lu", path.waypoints.size());
    // for (const auto &point : path.waypoints)
    // {
    //     gladiator->log("Waypoint: (%d, %d)", point.first, point.second);
    // }
    newPathSet = true;
    follow(path);
}

Path RobotController::pathTo(int i, int j)
{
    MazeSquare *position = gladiator->maze->getNearestSquare();
    if (position == nullptr)
    {
        gladiator->log("Position is null");
    }
    MazeSquare *goal = gladiator->maze->getSquare(i, j);
    if (goal == nullptr)
    {
        gladiator->log("Goal is null");
    }
    return aStar(gladiator, position, goal);
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

void RobotController::returnToMaze()
{
    returnToMazeMode = true;
}