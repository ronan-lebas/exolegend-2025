#include "aStar.h"

bool CompareNodes::operator()(const Node *a, const Node *b)
{
    return a->f() > b->f();
}

int manhattanDistance(MazeSquare *a, MazeSquare *b)
{
    return abs(a->i - b->i) + abs(a->j - b->j);
}

std::vector<MazeSquare *> reconstructPath(Node *node)
{
    std::vector<MazeSquare *> path;
    while (node)
    {
        path.push_back(node->square);
        node = node->parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

Path aStar(Gladiator *gladiator, MazeSquare *start, MazeSquare *goal)
{
    std::priority_queue<Node *, std::vector<Node *>, CompareNodes> openSet;
    std::unordered_map<MazeSquare *, Node *> allNodes;

    Node *startNode = new Node{start, 0, manhattanDistance(start, goal), nullptr};
    openSet.push(startNode);
    allNodes[start] = startNode;

    while (!openSet.empty())
    {
        Node *current = openSet.top();
        openSet.pop();

        if (current->square == goal)
        {
            std::vector<MazeSquare *> path = reconstructPath(current);
            for (auto &pair : allNodes)
                delete pair.second;
            Path path2 = Path();
            for (MazeSquare *square : path)
            {
                path2.waypoints.push_back(std::make_pair(square->i, square->j));
            }
            return path2;
        }

        MazeSquare *neighbors[] = {current->square->northSquare, current->square->southSquare,
                                   current->square->westSquare, current->square->eastSquare};

        bool hasNeighbourg = false;

        for (MazeSquare *neighbor : neighbors)
        {
            if (!neighbor || neighbor->danger > 0) // Avoid case with RED
                continue;
            int gNew = current->g + 1;
            hasNeighbourg = true;
            if (!allNodes.count(neighbor) || gNew < allNodes[neighbor]->g)
            {
                Node *neighborNode = new Node{neighbor, gNew, manhattanDistance(neighbor, goal), current};
                openSet.push(neighborNode);
                allNodes[neighbor] = neighborNode;
            }
        }

        if (!hasNeighbourg)
        {
            gladiator->log("No neighbourg");
            MazeSquare *west = gladiator->maze->getSquare(current->square->i - 1, current->square->j);
            MazeSquare *east = gladiator->maze->getSquare(current->square->i + 1, current->square->j);
            MazeSquare *north = gladiator->maze->getSquare(current->square->i, current->square->j - 1);
            MazeSquare *south = gladiator->maze->getSquare(current->square->i, current->square->j + 1);
            if (current->square->i > 6 && west != nullptr && west->danger == 0)
            {
                gladiator->log("Moving west");
                Node *westNode = new Node{west, current->g + 1, manhattanDistance(west, goal), current};
                openSet.push(westNode);
                allNodes[west] = westNode;
            } else if 
            (current->square->j > 6 && south != nullptr && south->danger == 0)
            {
                gladiator->log("Moving south");
                Node *southNode = new Node{south, current->g + 1, manhattanDistance(south, goal), current};
                openSet.push(southNode);
                allNodes[south] = southNode;
            } else if
            (current->square->j < 6 && north != nullptr && north->danger == 0)
            {
                gladiator->log("Moving north");
                Node *northNode = new Node{north, current->g + 1, manhattanDistance(north, goal), current};
                openSet.push(northNode);
                allNodes[north] = northNode;
            } 
            else if (east != nullptr && east->danger == 0) // Move east
            {
                gladiator->log("Moving east");
                Node *eastNode = new Node{east, current->g + 1, manhattanDistance(east, goal), current};
                openSet.push(eastNode);
                allNodes[east] = eastNode;
            } else {
                // West by default
                Node *westNode = new Node{west, current->g + 1, manhattanDistance(west, goal), current};
                openSet.push(westNode);
                allNodes[west] = westNode;
            }
        }
    }

    for (auto &pair : allNodes)
        delete pair.second;

    return {};
}
