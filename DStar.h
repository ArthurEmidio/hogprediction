//
//  DStar.hpp
//  BackgroundSubtraction
//
//  Created by Arthur Emídio Teixeira Ferreira on 11/3/15.
//  Copyright © 2015 Arthur Emídio Teixeira Ferreira. All rights reserved.
//

#ifndef DStar_h
#define DStar_h

#include <iostream>
#include <list>
#include <set>
#include <cmath>
#include <cassert>

#include "Graph.h"

using namespace std;

class DStar {
public:
    DStar(const Graph &graph);
    bool computeShortestPath();
    
private:
    double HEURISTIC_CONSTANT = sqrt(2.0) - 1.0;
    
    PriorityQueue U;
    
    const Graph &graph;
    
    double heuristic(Vertex *vertex, Vertex *start);
    
    pair<double, double> calcKey(Vertex *vertex, Vertex *start);
    
    PriorityQueue initialize();
    
    list<Edge*> successors(Vertex *u);
    
    list<Edge*> predecessors(Vertex *u);
    
    void updateVertex(Vertex *u);
    
};

#endif /* DStar_h */
