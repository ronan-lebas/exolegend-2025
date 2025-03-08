#include "dijkstra.h"

std::unordered_map<MazeSquare*, int> dijkstra(MazeSquare* start) {
    std::unordered_map<MazeSquare*, int> distances;
    std::priority_queue<std::pair<int, MazeSquare*>, 
                        std::vector<std::pair<int, MazeSquare*>>, 
                        std::greater<>> pq;
    
    distances[start] = 0;
    pq.push({0, start});
    
    while (!pq.empty()) {
        auto [currentDist, current] = pq.top();
        pq.pop();
        
        if (currentDist > distances[current]) continue;
        
        MazeSquare* neighbors[] = {current->northSquare, current->southSquare,
                                   current->westSquare, current->eastSquare};
        
        for (MazeSquare* neighbor : neighbors) {
            if (!neighbor) continue;
            int newDist = currentDist + 1;
            
            if (!distances.count(neighbor) || newDist < distances[neighbor]) {
                distances[neighbor] = newDist;
                pq.push({newDist, neighbor});
            }
        }
    }
    
    return distances;
}
