#ifndef AStar_h
#define AStar_h

#include <iostream>

#include "Graph.h"

using namespace std;

class AStar {
public:
    /*!
     * \brief Computes the shortest path using A*.
     * \param graph The graph. The start and goal nodes should be set before calling this method.
     */
    static bool computeShortestPath(const Graph &graph);
    
    /*!
     * \brief Computes the shortest path using A* (old version, not guaranteed to work).
     * \param graph The graph. The start and goal nodes should be set before calling this method.
     */
    static bool computeShortestPath2(const Graph &graph);
    
};

#endif /* AStar_h */
