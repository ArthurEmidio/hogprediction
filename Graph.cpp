//
//  Graph.cpp
//  BackgroundSubtraction
//
//  Created by Arthur Emídio Teixeira Ferreira on 11/3/15.
//  Copyright © 2015 Arthur Emídio Teixeira Ferreira. All rights reserved.
//

#include "Graph.h"
#include "Utils.h"

bool operator<=(const Key& lhs, const Key& rhs)
{
    return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second <= rhs.second);
}

bool operator<(const Key& lhs, const Key& rhs)
{
    return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second); // should check this
}

bool operator>=(const Key& lhs, const Key& rhs)
{
    return lhs.first > rhs.first || (lhs.first == rhs.first && lhs.second >= rhs.second);
}

bool operator>(const Key& lhs, const Key& rhs)
{
    return lhs.first > rhs.first || (lhs.first == rhs.first && lhs.second > rhs.second); // should check this
}

bool operator==(const Key& lhs, const Key& rhs)
{
    return lhs.first == rhs.first;
}

inline bool isElementInMatrix(int row, int col, vector<vector<Vertex*>> matrix)
{
    return row >= 0 && row < matrix.size() && col >= 0 && col < matrix[row].size();
}

Graph::Graph(const vector<vector<shared_ptr<Cell>>> &matrix)
{
    vector<vector<Vertex*>> vertices;
    
    int i = 0;
    for (const vector<shared_ptr<Cell>> &cells : matrix) {
        int j = 0;
        vector<Vertex*> row;
        
        for (shared_ptr<Cell> cell : cells) {
            Vertex *currVertex = new Vertex();
            this->vertices.push_back(currVertex);
            row.push_back(currVertex);
            
            if (cell->start) {
                this->start = currVertex;
            }
            
            if (cell->goal) {
                this->goal = currVertex;
            }
            
            currVertex->location = cell->position;
            currVertex->row = i;
            currVertex->col = j;
            
            currVertex->visited = false;
            currVertex->cell = cell;
            j++;
        }
        
        vertices.push_back(row);
        i++;
    }
    
    for (const vector<Vertex*> &row : vertices) {
        for (Vertex *vertex : row) {
            for (int i = vertex->row - 1; i <= vertex->row + 1; i++) {
                for (int j = vertex->col - 1; j <= vertex->col + 1; j++) {
                    if (isElementInMatrix(i, j, vertices) && !(i == vertex->row && j == vertex->col)) {
                        Vertex *neighbor = vertices[i][j];
                        Edge *edge = new Edge();
                        
                        if (neighbor->occupied || vertex->occupied) {
                            edge->cost = INFINITY;
                        } else {
                            vector<double> subtraction = Utils::subtractVectors(neighbor->cell->descriptorValues,
                                                                                vertex->cell->descriptorValues);
                            edge->cost = 0;
                            for (const double &element : subtraction) {
                                edge->cost += element;
                            }
                        }
                        
                        edge->vertex = neighbor;
                        vertex->edges.push_back(edge);
                    }
                }
            }
        }
    }    
}
