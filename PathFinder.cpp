//
//  PathFinder.cpp
//  BackgroundSubtraction
//
//  Created by Arthur Emídio Teixeira Ferreira on 9/7/15.
//  Copyright (c) 2015 Arthur Emídio Teixeira Ferreira. All rights reserved.
//

#include "PathFinder.h"
#include "Utils.h"
#include "AStar.h"
#include "Dijkstra.h"
#include "DStar.h"

PathFinder::PathFinder(vector<vector<shared_ptr<Cell>>> fieldOfView) : fieldOfView(fieldOfView)
{
}

void PathFinder::drawShortestPaths(Mat img, Graph &graph, const Point &goal, const Point &start, Scalar color,
                                   ShortestPathAlgorithm algorithm, vector<Point> &path)
{
    Vertex *startVertex = nullptr;
    double minDistStart = INFINITY;
    Vertex *goalVertex = nullptr;
    double minDistGoal = INFINITY;
    
    // Finding the corresponding start and goal vertices
    for (Vertex *vertex : graph.vertices) {
        double distFromStart = hypot(vertex->location.x - start.x, vertex->location.y - start.y);
        double distFromGoal = hypot(vertex->location.x - goal.x, vertex->location.y - goal.y);
        
        if (distFromStart < minDistStart) {
            startVertex = vertex;
            minDistStart = distFromStart;
        }
        
        if (distFromGoal < minDistGoal) {
            goalVertex = vertex;
            minDistGoal = distFromGoal;
        }
    }
    
    graph.start = startVertex;
    graph.goal = goalVertex;
    
    // Computing the shortest path
    if (algorithm == ShortestPathAlgorithm::D_STAR) {
        DStar dstar(graph);
        bool success = dstar.computeShortestPath();
        
        if (success) {
            Vertex *vertex = graph.start;
            Point p1;
            
            if (vertex != graph.goal) {
                p1 = vertex->cell->position;
                path.push_back(p1);
            }
            
            while (vertex != graph.goal) {
                vertex->visited = true;
                
                double minVal = INFINITY;
                Vertex *next = nullptr;
                for (Edge *edge : vertex->edges) {
                    Vertex *neighbor = edge->vertex;
                    double currValue = neighbor->g_value + edge->cost;
                    if (currValue < minVal && !neighbor->visited) { // should I continue using this condition?
                        next = neighbor;
                        minVal = currValue;
                    }
                }
                
                if (next != nullptr) {
                    Point p2 = next->cell->position;
                    line(img, p1, p2, color, 5);
                    p1 = p2;
                }
                
                vertex = next;
            }
        }
    } else if (algorithm == ShortestPathAlgorithm::A_STAR) {
        bool success = AStar::computeShortestPath(graph);
        double pathSize = 0;
        
        if (success) {
            Vertex *vertex = graph.goal;
            Point p1, p2;
            
            if (vertex != graph.start) {
                p1 = vertex->location;
                path.push_back(p1);
                Vertex *v2 = vertex;
                Vertex *v1 = vertex->parent;
                
                vertex = vertex->parent;
                
                if (v1 != nullptr) {
                    for (Edge *edge : v1->edges) {
                        if (edge->vertex == v2) {
                            pathSize += edge->cost;
                            break;
                        }
                    }
                }

            }
            
            while (vertex != graph.start) {
                p2 = vertex->location;
                line(img, p1, p2, color, 5);
                path.push_back(p2);
                p1 = p2;
                Vertex *v2 = vertex;
                Vertex *v1 = vertex->parent;
                
                vertex = vertex->parent;
                
                if (v1 != nullptr) {
                    for (Edge *edge : v1->edges) {
                        if (edge->vertex == v2) {
                            pathSize += edge->cost;
                            break;
                        }
                    }
                }
            }
        }
    } else if (algorithm == ShortestPathAlgorithm::DIJKSTRA) {
        bool success = Dijkstra::computeShortestPath(graph);
        double pathSize = 0;
        
        if (success) {
            Vertex *vertex = graph.goal;
            Point p1, p2;
            
            if (vertex != graph.start) {
                p1 = vertex->location;
                path.push_back(p1);
                
                Vertex *v2 = vertex;
                Vertex *v1 = vertex->parent;
                
                vertex = vertex->parent;
                
                if (v1 != nullptr) {
                    for (Edge *edge : v1->edges) {
                        if (edge->vertex == v2) {
                            pathSize += edge->cost;
                            break;
                        }
                    }
                }
            }
            
            while (vertex != graph.start) {
                p2 = vertex->location;
                line(img, p1, p2, color, 5);
                path.push_back(p2);
                p1 = p2;
                
                Vertex *v2 = vertex;
                Vertex *v1 = vertex->parent;
                
                vertex = vertex->parent;
                
                if (v1 != nullptr) {
                    for (Edge *edge : v1->edges) {
                        if (edge->vertex == v2) {
                            pathSize += edge->cost;
                            break;
                        }
                    }
                }
                
            }
        }
                
//        Dijkstra dijkstra(graph);
//        set<Vertex*> visited;
//        bool success = dijkstra.computeShortestPath();
//        if (success) {
//            Vertex *vertex = graph.goal;
//            visited.insert(vertex);
//            
//            while (vertex != graph.start) {
//                double minVal = INFINITY;
//                Vertex *nextVertex = nullptr;
//                
//                for (Edge *edge : vertex->edges) {
//                    Vertex *neighbor = edge->vertex;
//                    if (neighbor->distance < minVal) {
//                        minVal = neighbor->distance;
//                        nextVertex = neighbor;
//                    }
//                }
//                
//                line(img, vertex->location, nextVertex->location, color, 5);
//                vertex = nextVertex;
//                visited.insert(vertex);
//            }
//        }
    }
}

inline bool PathFinder::doesPointExist(int row, int col) const
{
    return row >= 0 && col >= 0 && row < fieldOfView.size() && col < fieldOfView[row].size();
}

vector<pair<Point, double>> PathFinder::computeNeighborhoodDiffence(int row, int col, double &avgDiff)
{
    vector<pair<Point, double>> neighborhood;
    
    double subtractions = 0;
    for (int i = (row - 1); i <= (row + 1); i++) {
        for (int j = (col - 1); j <= (col + 1); j++) {
            if (i == row && j == col) continue; // the center point cannot be its own neighbor
            if (!doesPointExist(i, j)) continue; // discarding invalid neighbors
            
            vector<double> subtraction = Utils::subtractVectors(fieldOfView[row][col]->descriptorValues,
                                                                fieldOfView[i][j]->descriptorValues);

            double score = 0;
            for(double element : subtraction) score += element;
            subtractions += score;
            
            neighborhood.push_back(pair<Point, double>(Point(i,j), score));
        }
    }
    
    avgDiff = subtractions / neighborhood.size();
    
    return neighborhood;
}

void PathFinder::calculateScores()
{
    int i = 0;
    for (const vector<shared_ptr<Cell>> &straightLine : fieldOfView) {
        for (int j = 0; j < straightLine.size(); j++) {
            double avgDiff;
            vector<pair<Point, double>> neighbors = computeNeighborhoodDiffence(i, j, avgDiff);
            
            for (const pair<Point, double> &neighbor : neighbors) {
                if (neighbor.second <= avgDiff) {
                    fieldOfView[neighbor.first.x][neighbor.first.y]->score += 1;
                }
            }
        }
        i++;
    }
}

// Draws all cells with a score greater or equal to score
void PathFinder::drawOptimalCells(Mat img, int score, Scalar color)
{
    calculateScores();
    
    for (const vector<shared_ptr<Cell>> &straightLine : fieldOfView) {
        for (shared_ptr<Cell> cell : straightLine) {
            if (cell->score >= score) circle(img, cell->position, 3, color, -1);
        }
    }
}

// Naive method

// Rates all the naive paths (ie. straight paths) and draws the 'n' most optimal ones into 'img'
void PathFinder::drawNaivePaths(Mat img, int n, Scalar color)
{
    vector<pair<vector<shared_ptr<Cell>>, double>> optimalPaths = findNaiveOptimalPaths();
    
    int i = 0;
    for (const pair<vector<shared_ptr<Cell>>, double> &pair : optimalPaths) {
        for (shared_ptr<Cell> c : pair.first) {
            circle(img, c->position, 3, color, -1);
        }
        
        if (++i == n) break;
    }
}

// It takes all the straight lines from the field of view and consider them as the only possible paths
// Then, it sorts the paths based on the HOG variation
vector<pair<vector<shared_ptr<Cell>>, double>> PathFinder::findNaiveOptimalPaths()
{
    vector<pair<vector<shared_ptr<Cell>>, double>> paths(fieldOfView.size());
    
    int i = 0;
    for(vector<shared_ptr<Cell>> path : fieldOfView) {
        paths[i].first = path;
        
        float score = 0;
        for(int j = 1; j < path.size(); j++) {
            vector<double> sub = Utils::subtractVectors(path[j]->descriptorValues, path[j-1]->descriptorValues);
            for(double elm : sub) score += elm;
        }
        
        paths[i].second = score;
        i++;
    }
    
    sort(paths.begin(), paths.end(), compareNaiveScores);
    
    return paths;
}

// Compare function to sort elements in ascending order
bool PathFinder::compareNaiveScores(pair<vector<shared_ptr<Cell>>, double> pair1,
                                    pair<vector<shared_ptr<Cell>>, double> pair2)
{
    return pair1.second < pair2.second;
}
