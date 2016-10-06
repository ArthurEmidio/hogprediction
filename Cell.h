//
//  Cell.h
//  BackgroundSubtraction
//
//  Created by Arthur Emídio Teixeira Ferreira on 9/7/15.
//  Copyright (c) 2015 Arthur Emídio Teixeira Ferreira. All rights reserved.
//

#ifndef __BackgroundSubtraction__Cell__
#define __BackgroundSubtraction__Cell__

#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class Cell {
public:
    Point position;
    vector<float> descriptorValues;
    int score;
    bool start;
    bool goal;
    
    Cell(Point position, vector<float> descriptorValues);
    
    static bool doesPointExist(int row, int col, vector<vector<shared_ptr<Cell>>> fieldOfView);
    
    static vector<shared_ptr<Cell>> computeNeighborhoodDiffence(Point location,
                                                                vector<vector<shared_ptr<Cell>>> fieldOfView);
    
};

#endif /* defined(__BackgroundSubtraction__Cell__) */
