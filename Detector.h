//
//  Detector.h
//  BackgroundSubtraction
//
//  Created by Arthur Emídio Teixeira Ferreira on 3/9/15.
//  Copyright (c) 2015 Arthur Emídio Teixeira Ferreira. All rights reserved.
//

#ifndef __BackgroundSubtraction__Detector__
#define __BackgroundSubtraction__Detector__

#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class Detector {

private:
    int method;
    
    vector<vector<Point>> allTargets;
    vector<vector<Point>> validTargets;
    vector<vector<Point>> invalidTargets;
    
    vector<Mat> frames;
    
    Mat backgroundSubtraction();
    vector<vector<Point>> getContours(Mat img);
    
public:
    Detector(vector<Mat> frames, int method = 1);
    
    vector<vector<Point>> getAllTargets();
    vector<vector<Point>> getValidTargets();
    vector<vector<Point>> getInvalidTargets();
    
};

#endif /* defined(__BackgroundSubtraction__Detector__) */
