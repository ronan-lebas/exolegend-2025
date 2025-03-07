#include "aStar.h"

bool CompareNodes::operator()(const Node* a, const Node* b) {
        return a->f() > b->f();
    }

int manhattanDistance(MazeSquare* a, MazeSquare* b) {
    return abs(a->i - b->i) + abs(a->j - b->j);
}

std::vector<MazeSquare*> reconstructPath(Node* node) {
    std::vector<MazeSquare*> path;
    while (node) {
        path.push_back(node->square);
        node = node->parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

Path aStar(MazeSquare* start, MazeSquare* goal) {
    std::priority_queue<Node*, std::vector<Node*>, CompareNodes> openSet;
    std::unordered_map<MazeSquare*, Node*> allNodes;
    
    Node* startNode = new Node{start, 0, manhattanDistance(start, goal), nullptr};
    openSet.push(startNode);
    allNodes[start] = startNode;
    
    while (!openSet.empty()) {
        Node* current = openSet.top();
        openSet.pop();
        
        if (current->square == goal) {
            std::vector<MazeSquare*> path = reconstructPath(current);
            for (auto& pair : allNodes) delete pair.second;
            Path path2 = Path();
            for (MazeSquare* square : path) {
                path2.waypoints.push_back(std::make_pair(square->i, square->j));
            }
            return path2;
        }
        
        MazeSquare* neighbors[] = {current->square->northSquare, current->square->southSquare,
                                   current->square->westSquare, current->square->eastSquare};
        
        for (MazeSquare* neighbor : neighbors) {
            if (!neighbor) continue;
            int gNew = current->g + 1;
            
            if (!allNodes.count(neighbor) || gNew < allNodes[neighbor]->g) {
                Node* neighborNode = new Node{neighbor, gNew, manhattanDistance(neighbor, goal), current};
                openSet.push(neighborNode);
                allNodes[neighbor] = neighborNode;
            }
        }
    }
    
    for (auto& pair : allNodes) delete pair.second;

    return {};
}
