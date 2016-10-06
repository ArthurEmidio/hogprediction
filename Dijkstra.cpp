//
//  Dijkstra.cpp
//  BackgroundSubtraction
//
//  Created by Arthur Emídio Teixeira Ferreira on 8/2/16.
//  Copyright © 2016 Arthur Emídio Teixeira Ferreira. All rights reserved.
//

#include "Dijkstra.h"

bool Dijkstra::computeShortestPath(const Graph &graph)
{
    graph.start->distance = 0;
    
    priority_queue<Vertex*, vector<Vertex*>, function<bool(Vertex*, Vertex*)>> queue([](Vertex *v1, Vertex *v2) {
        return v1->distance > v2->distance;
    });
    
    queue.push(graph.start);
    
    while (!queue.empty()) {
        Vertex *currVertex = queue.top();
        queue.pop();
        if (currVertex->visited) continue;
        
        currVertex->visited = true;
        for (Edge *edge : currVertex->edges) {
            Vertex *neighbor = edge->vertex;
            double distance = currVertex->distance + edge->cost;
            if (neighbor->distance > distance) {
                neighbor->distance = distance;
                neighbor->parent = currVertex;
            }
            if (!neighbor->visited) queue.push(neighbor);
        }
    }
        
    return true;
}
