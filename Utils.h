//
//  Utils.h
//  BackgroundSubtraction
//
//  Created by Arthur Emídio Teixeira Ferreira on 3/10/15.
//  Copyright (c) 2015 Arthur Emídio Teixeira Ferreira. All rights reserved.
//

#ifndef __BackgroundSubtraction__Utils__
#define __BackgroundSubtraction__Utils__

#include "opencv2/opencv.hpp"
#include "Cell.h"

using namespace std;
using namespace cv;

class Utils {
public:
    static vector<Rect> getRectanglesFromContours(vector<vector<Point>> contours);
    
    static bool contains(Rect container, Rect contained);
    
    static vector<double> subtractVectors(vector<float> v1, vector<float> v2);
    
    static void drawFieldOfView(vector<vector<shared_ptr<Cell>>> fieldOfView, Mat img, Scalar color);
    
    static void drawGraph(vector<Point2f> points, string xLabel, string yLabel);
    
    static double distFromNearestPoint(Point point, vector<Point> points, Point &nearestPoint);
    
    static void savePointsToCsv(vector<Point2f> points, string path, string fileName);
    
    static Mat get_hogdescriptor_visual_image(Mat& origImg,
                                       vector<float>& descriptorValues,
                                       Size winSize,
                                       Size cellSize,
                                       int scaleFactor,
                                       double viz_factor, Mat &visual_image);
    
};

#endif /* defined(__BackgroundSubtraction__Utils__) */
