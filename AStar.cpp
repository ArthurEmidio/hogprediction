#include "AStar.h"

#include <queue>
#include <functional>
#include <vector>

struct VertexCompare {
    bool operator() (const Vertex* lhs, const Vertex* rhs) const {
        double lhs_f_value = lhs->g_value + lhs->h_value;
        double rhs_f_value = rhs->g_value + rhs->h_value;
        return lhs_f_value < rhs_f_value;
    }
};

inline double _heuristic(Vertex *vertex, const Graph &graph)
{
//    int dx = abs(vertex->row - graph.goal->row);
//    int dy = abs(vertex->col - graph.goal->col);
//    return (dx + dy) * 0.5;
    return 0;
}

bool AStar::computeShortestPath(const Graph &graph)
{
    priority_queue<Vertex*, vector<Vertex*>, function<bool(Vertex*, Vertex*)>> open([](Vertex *lhs, Vertex *rhs) {
        double lhs_f_value = lhs->g_value + lhs->h_value;
        double rhs_f_value = rhs->g_value + rhs->h_value;
        return lhs_f_value > rhs_f_value;
    });
    
    set<Vertex*> inOpen;
    set<Vertex*> closed;
    
    graph.start->g_value = 0;
    graph.start->h_value = _heuristic(graph.start, graph);
    open.push(graph.start);
    inOpen.insert(graph.start);
    
    while (!open.empty()) {
        Vertex *current = open.top();
        open.pop();
        inOpen.erase(current);
        
        if (current == graph.goal) {
            break;
        }
        
        if (closed.count(current) == 0) {
            closed.insert(current);
            
            for (Edge *edge : current->edges) {
                Vertex *neighbor = edge->vertex;
                if (closed.count(neighbor) == 0) {
                    double g_score = current->g_value + edge->cost;                    
                    if (inOpen.count(neighbor) == 0 || g_score < neighbor->g_value) {
                        neighbor->parent = current;
                        neighbor->g_value = g_score;
                        neighbor->h_value = _heuristic(neighbor, graph);
                        open.push(neighbor);
                        inOpen.insert(neighbor);
                    }
                }
            }
        }
    }
    
    return true;
}

bool AStar::computeShortestPath2(const Graph &graph)
{
    map<Vertex*, set<Vertex*, VertexCompare>::iterator> openMap;
    multiset<Vertex*, vertexcomp> open;
    set<Vertex*> closed;
    
    graph.start->g_value = 0;
    open.insert(graph.start);
    
    while (!open.empty() && *open.begin() != graph.goal) {
        Vertex *currentVertex = *open.begin();
        
        openMap.erase(currentVertex);
        open.erase(open.begin());
        closed.insert(currentVertex);
        
        for (Edge *edge : currentVertex->edges) {
            Vertex *neighbor = edge->vertex;
            double cost = currentVertex->g_value + edge->cost;
            
            auto neighborInOpenSetIterator = openMap.find(neighbor);
            bool isNeighborInOpenSet = neighborInOpenSetIterator != openMap.end();
            
            auto neighborInClosedSetIterator = closed.find(neighbor);
            bool isNeighborInClosedSet = neighborInClosedSetIterator != closed.end();
            
            if (cost < neighbor->g_value) {
                if (isNeighborInOpenSet) {
                    openMap.erase(neighborInOpenSetIterator);
                    open.erase(neighborInOpenSetIterator->second);
                    isNeighborInOpenSet = false;
                } else if (isNeighborInClosedSet) {
                    closed.erase(neighborInClosedSetIterator);
                    isNeighborInClosedSet = false;
                }
            }
            
            if (!isNeighborInOpenSet && !isNeighborInClosedSet) {
                neighbor->g_value = cost;
                neighbor->parent = currentVertex;
                set<Vertex*, vertexcomp>::iterator it = open.insert(neighbor);
                openMap[neighbor] = it;
            }
        }
    }
    
    return true;
}
