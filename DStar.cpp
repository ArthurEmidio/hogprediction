//
//  DStar.cpp
//  BackgroundSubtraction
//
//  Created by Arthur Emídio Teixeira Ferreira on 11/3/15.
//  Copyright © 2015 Arthur Emídio Teixeira Ferreira. All rights reserved.
//

#include "DStar.h"

DStar::DStar(const Graph &graph) : graph(graph)
{
    this->U = initialize();
}

double DStar::heuristic(Vertex *vertex, Vertex *start)
{
    int dx = abs(vertex->row - start->row);
    int dy = abs(vertex->col - start->col);
    
    int maxValue, minValue;
    if (dx > dy) {
        maxValue = dx;
        minValue = dy;
    } else {
        maxValue = dy;
        minValue = dx;
    }
    
    return minValue*HEURISTIC_CONSTANT + maxValue;
}

pair<double, double> DStar::calcKey(Vertex *vertex, Vertex *start)
{
    double min_value = min(vertex->g_value, vertex->rhs_value);
    return pair<double, double>(min_value + heuristic(vertex, start), min_value);
}

PriorityQueue DStar::initialize()
{
    assert(graph.start != NULL);
    assert(graph.goal != NULL);
    assert(graph.vertices.size() > 0);
    
    PriorityQueue U;
    
    for (Vertex *vertex : graph.vertices) {
        vertex->rhs_value = INFINITY;
        vertex->g_value = INFINITY;
    }
    
    graph.goal->rhs_value = 0;
    graph.goal->priority = calcKey(graph.goal, graph.start);
    U.insert(graph.goal);
    
    return U;
}

list<Edge*> DStar::successors(Vertex *u)
{
    list<Edge*> result;
    if (u->occupied) return result;
    
    
    for (Edge *edge : u->edges) {
        result.push_back(edge);
    }
    
    return result;
}

list<Edge*> DStar::predecessors(Vertex *u)
{
    list<Edge*> result;
    
    for (Edge *edge : u->edges) {
        if (edge->vertex->occupied == false) {
            result.push_back(edge);
        }
    }
    
    return result;
}

void DStar::updateVertex(Vertex *u)
{
    if (u != graph.goal) {
        double minValue = INFINITY;
        for (Edge *e : successors(u)) {
            minValue = min(e->vertex->g_value + e->cost, minValue); // gotta change this
        }
        u->rhs_value = minValue;
    }
    
    // if true, u is in U
    PriorityQueue::iterator u_iterator = U.find(u);
    if (u_iterator != U.end()) {
        U.erase(u_iterator);
    }
    
    if (u->g_value != u->rhs_value) {
        u->priority = calcKey(u, graph.start);
        U.insert(u);
    }
}

bool DStar::computeShortestPath()
{
    while ((*(U.begin()))->priority < calcKey(graph.start, graph.start) || graph.start->rhs_value != graph.start->g_value) { // modified this, it was previously using the LPA* logic
        if (U.size() == 0) {
            return false;
        }
        
        Vertex *u = *(U.begin());
        U.erase(U.begin());
        
        if (u->g_value > u->rhs_value) {
            u->g_value = u->rhs_value;
            for (Edge *e : predecessors(u)) {
                updateVertex(e->vertex);
            }
        } else {
            u->g_value = INFINITY;
            for (Edge *e : predecessors(u)) {
                updateVertex(e->vertex);
            }
            updateVertex(u);
        }
    }
    
    return true;
}
