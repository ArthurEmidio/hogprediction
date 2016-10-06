//
//  Cell.cpp
//  BackgroundSubtraction
//
//  Created by Arthur Emídio Teixeira Ferreira on 9/7/15.
//  Copyright (c) 2015 Arthur Emídio Teixeira Ferreira. All rights reserved.
//

#include "Cell.h"

Cell::Cell(Point position, vector<float> descriptorValues) :
    position(position),
    descriptorValues(descriptorValues),
    score(0),
    start(false),
    goal(false)
{
}

// Helper function to check if a Cell exists in fieldOfView at coordinate (row,col)
bool Cell::doesPointExist(int row, int col, vector<vector<shared_ptr<Cell>>> fieldOfView)
{
    return row >= 0 && col >= 0 && row < fieldOfView.size() && col < fieldOfView[row].size();
}

// Returns the Cell's neighbors (not more than 8) at the matrix coordinate 'location'
vector<shared_ptr<Cell>> Cell::computeNeighborhoodDiffence(Point location, vector<vector<shared_ptr<Cell>>> fieldOfView)
{
    vector<shared_ptr<Cell>> neighborhood;
    
    for(int i = (location.x - 1); i <= (location.x + 1); i++) {
        for(int j = (location.y - 1); j <= (location.y + 1); j++) {
            if (Point(i,j) == location) continue; // the center point cannot be its own neighbor
            if (!doesPointExist(i, j, fieldOfView)) continue; // discarding invalid neighbors
            neighborhood.push_back(fieldOfView[i][j]);
        }
    }
    
    return neighborhood;
}
