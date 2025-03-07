#ifndef A_STAR_H
#define A_STAR_H

#include "gladiator.h"
#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include "RobotController.h"

struct Node {
    MazeSquare* square;
    int g, h;
    Node* parent;
    int f() const { return g + h; }
};

struct CompareNodes {
    bool operator()(const Node* a, const Node* b);
};

int manhattanDistance(MazeSquare* a, MazeSquare* b);

std::vector<MazeSquare*> reconstructPath(Node* node);

Path aStar(MazeSquare* start, MazeSquare* goal);

#endif