//
//  MovementDetector.h
//  BackgroundSubtraction
//
//  Created by Arthur Emídio Teixeira Ferreira on 9/6/15.
//  Copyright (c) 2015 Arthur Emídio Teixeira Ferreira. All rights reserved.
//

#ifndef __BackgroundSubtraction__MovementDetector__
#define __BackgroundSubtraction__MovementDetector__

#include "Cell.h"

#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class MovementDetector {
public:
    static vector<vector<shared_ptr<Cell>>> getFieldOfView(const Rect &target, const Mat &firstFrame,
                                                           const Mat &lastFrame, int degrees, int spacing,
                                                           const Size &cellSize);
    
    static vector<vector<shared_ptr<Cell>>> getFieldOfView(const Rect &targetBefore, const Rect &targetAfter,
                                                           Mat frameAfter, int degrees, int spacing,
                                                           const Size &cellSize);
    
private:
    static vector<vector<shared_ptr<Cell>>> getFieldOfViewHelper(Point2f previousPosition, Point2f nextPosition,
                                                                 Mat frameAfter, int degrees, int spacing,
                                                                 const Size &cellSize);
    
    static vector<float> computeHOG(const Point &point, const Mat &frame, const Size &cellSize);
    
    static void getMovementData(const Rect &target, const Mat &firstFrame, const Mat &lastFrame,
                                Point2f &previousPosition, Point2f &nextPosition);

};

#endif /* defined(__BackgroundSubtraction__MovementDetector__) */
