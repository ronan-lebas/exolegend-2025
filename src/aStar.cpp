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
    gladiator->log("A* from (%d, %d) to (%d, %d)", start->i, start->j, goal->i, goal->j);
    delay(100);
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
            gladiator->log("Finished A*");
            return path2;
        }

        MazeSquare *neighbors[] = {current->square->northSquare, current->square->southSquare,
                                   current->square->westSquare, current->square->eastSquare};
        //gladiator->log("Current square: %d, %d", current->square->i, current->square->j);
        for (int index = 0; index < 4; index++)
        {
            //gladiator->log("index: %d", index);
            MazeSquare *neighbor = neighbors[index];
            int gNew;
            if (neighbor && neighbor->danger == 0)
            {
                //gladiator->log("No danger and neighbor");
                gNew = current->g + 1;
            }
            else
            {
                if (!neighbor)
                {
                    //gladiator->log("No neighbor");
                    gNew = current->g + PATH_WALL_WEIGHT;
                    if (index == 0)
                    {
                        //gladiator->log("no neighbor at north");
                        MazeSquare *north = gladiator->maze->getSquare(current->square->i, current->square->j + 1);
                        if (north)
                        {
                            neighbors[index] = north;
                            //gladiator->log("north found");
                        }
                        else
                        {
                            //gladiator->log("north not found");
                        }
                    }
                    else if (index == 1)
                    {
                        //gladiator->log("no neighbor at south");
                        MazeSquare *south = gladiator->maze->getSquare(current->square->i, current->square->j - 1);
                        if (south)
                        {
                            neighbors[index] = south;
                            //gladiator->log("south found");
                        }
                        else
                        {
                            //gladiator->log("south not found");
                        }
                    }
                    else if (index == 2)
                    {
                        //gladiator->log("no neighbor at west");
                        MazeSquare *west = gladiator->maze->getSquare(current->square->i - 1, current->square->j);
                        if (west)
                        {
                            neighbors[index] = west;
                            //gladiator->log("west found");
                        }
                        else
                        {
                            //gladiator->log("west not found");
                        }
                    }
                    else
                    {
                        MazeSquare *east = gladiator->maze->getSquare(current->square->i + 1, current->square->j);
                        gladiator->log("no neighbor at east");
                        if (east)
                        {
                            neighbors[index] = east;
                            //gladiator->log("east found");
                        }
                        else
                        {
                            //gladiator->log("east not found");
                        }
                    }

                    neighbor = neighbors[index];
                }
                else
                { // danger
                    //gladiator->log("Danger neighbor");
                    switch(neighbor->danger) {
                        case 0:
                        case 1:
                        case 2:
                            gNew = current->g + PATH_BOMB_CHILL_WEIGHT;
                            break;
                        case 3:
                        case 4:
                        case 5:
                            gNew = current->g + PATH_BOMB_URGENT_WEIGHT;
                            break;
                        case 6:
                        case 7:
                        default:
                            gNew = current->g + PATH_BOMB_IMMINENT_WEIGHT;
                            break;
                    }
                }
            }

            if (neighbor) // If neighbor has been created
            {
                if (!allNodes.count(neighbor) || gNew < allNodes[neighbor]->g)
                {
                    Node *neighborNode = new Node{neighbor, gNew, manhattanDistance(neighbor, goal), current};
                    openSet.push(neighborNode);
                    allNodes[neighbor] = neighborNode;
                }
            }
        }
    }

    for (auto &pair : allNodes)
        delete pair.second;

    gladiator->log("Finished A*");
    return {};
}
