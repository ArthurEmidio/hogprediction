//
//  Graph.hpp
//  BackgroundSubtraction
//
//  Created by Arthur Emídio Teixeira Ferreira on 11/3/15.
//  Copyright © 2015 Arthur Emídio Teixeira Ferreira. All rights reserved.
//

#ifndef Graph_h
#define Graph_h

#include <iostream>
#include <list>
#include <set>
#include <cmath>
#include <cassert>

#include "Cell.h"

using namespace std;

struct Vertex;
struct Edge;
struct Graph;

typedef pair<double, double> Key;

struct Edge {
    double cost;
    Vertex *vertex;
    struct Edge *inverseEdge;
};

struct Vertex {
    list<Edge*> edges;
    Key priority; // used in the priority queue
    double g_value = INFINITY;
    double rhs_value;
    bool occupied;
    
    Point location; // pixel-location
    
    int row;
    int col;
    
    bool visited;
    
    // info about each vertex
    shared_ptr<Cell> cell;
    
    // used in A*
    double h_value = INFINITY;
    Vertex *parent = nullptr;
    bool isInPath = false; // temporary variable
    
    // Used in Dijkstra
    double distance = INFINITY;
};

struct Graph {
    Graph(const vector<vector<shared_ptr<Cell>>> &matrix);
    
    list<Vertex*> vertices;
    Vertex *start;
    Vertex *goal;
};

struct vertexcomp {
    bool operator() (const Vertex* lhs, const Vertex* rhs) const {
        return lhs->priority < rhs->priority;
    }
};

typedef set<Vertex*, vertexcomp> PriorityQueue;

#endif /* Graph_hpp */
