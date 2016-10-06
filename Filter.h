//
//  Filter.h
//  BackgroundSubtraction
//
//  Created by Arthur Emídio Teixeira Ferreira on 3/6/15.
//  Copyright (c) 2015 Arthur Emídio Teixeira Ferreira. All rights reserved.
//

#ifndef __BackgroundSubtraction__Filter__
#define __BackgroundSubtraction__Filter__

#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class Filter {
    
private:
    Mat image;
    int CONSTANT;
    vector<vector<Point>> contours;
    vector<vector<Point>> validContours;
    vector<vector<Point>> invalidContours;
    vector<pair<vector<Point>, double>> pairs;
    
    void obtainAreas(double *smallestArea, double *biggestArea);
    void obtainScores(double smallestArea, double biggestArea);
    static bool compareScores(pair<vector<Point>, double> pair1, pair<vector<Point>, double> pair2);
    void sortPairsByScore();
    void classifyContours();
    
    
public:
    Filter(Mat image, vector<vector<Point>> contours, int C=1000);
    
    vector<vector<Point>> getAllContours();
    vector<vector<Point>> getValidContours();
    vector<vector<Point>> getInvalidContours();
};



#endif /* defined(__BackgroundSubtraction__Filter__) */
