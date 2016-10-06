//
//  Dijkstra.h
//  BackgroundSubtraction
//
//  Created by Arthur Emídio Teixeira Ferreira on 8/2/16.
//  Copyright © 2016 Arthur Emídio Teixeira Ferreira. All rights reserved.
//

#ifndef Dijkstra_h
#define Dijkstra_h

#include "Graph.h"

#include <functional>
#include <queue>

class Dijkstra {
public:
    /*!
     * \brief Computes the shortest path in a graph using the Dijkstra algorithm.
     * \param graph The graph. The start and goal nodes need to be set before calling this method.
     */
    static bool computeShortestPath(const Graph &graph);
    
};

#endif /* Dijkstra_h */
