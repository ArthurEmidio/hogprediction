//
//  Detector.cpp
//  BackgroundSubtraction
//
//  Created by Arthur Emídio Teixeira Ferreira on 3/9/15.
//  Copyright (c) 2015 Arthur Emídio Teixeira Ferreira. All rights reserved.
//

#include "Detector.h"
#include "Filter.h"
#include "Utils.h"

// Public methods

Detector::Detector(vector<Mat> frames, int method) {
    this->method = method;
    this->frames = frames;
    
    Mat result = backgroundSubtraction();
    
    allTargets = getContours(result);
    
    Filter filter = Filter(frames[0], allTargets);

    validTargets = filter.getValidContours();
    invalidTargets = filter.getInvalidContours();
}

vector<vector<Point>> Detector::getAllTargets() {
    return allTargets;
}

vector<vector<Point>> Detector::getValidTargets() {
    return validTargets;
}

vector<vector<Point>> Detector::getInvalidTargets() {
    return invalidTargets;
}

// Private methods

Mat Detector::backgroundSubtraction() {
    Ptr<BackgroundSubtractor> pMog;
    
    if (method == 1) {
        pMog = new BackgroundSubtractorMOG();
    } else if (method == 2) {
        pMog = new BackgroundSubtractorMOG2();
    } else if(method == 3) {
        pMog = new BackgroundSubtractorGMG();
    } else {
        CV_Error(-206, "Invalid Background Subtraction method");
    }
    
    Mat result;
    
    int fileCounter = 0;
    for(Mat currentFrame : frames) {
        pMog->operator()(currentFrame, result);
        fileCounter++;
    }
    
    return result;
}

vector<vector<Point>> Detector::getContours(Mat img) {
    Mat edges;
    Canny(img, edges, 10, 20);
    
    Mat strel = getStructuringElement(MORPH_RECT, Size(20, 20));
    morphologyEx(edges, edges, MORPH_CLOSE, strel);
    
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours(edges, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0,0));
    
    return contours;
}
