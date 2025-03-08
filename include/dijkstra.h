#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <unordered_map>
#include "gladiator.h"
#include <queue>
#include <vector>

std::unordered_map<MazeSquare*, int> dijkstra(MazeSquare* start);

#endif