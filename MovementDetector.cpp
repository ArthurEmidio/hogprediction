//
//  MovementDetector.cpp
//  BackgroundSubtraction
//
//  Created by Arthur Emídio Teixeira Ferreira on 9/6/15.
//  Copyright (c) 2015 Arthur Emídio Teixeira Ferreira. All rights reserved.
//

#include "MovementDetector.h"
#include "Utils.h"

// Computes the field of view.
// The field of view is composed of N straight lines (N is given by 'degrees', such that 1 <= N <= 360)
// Each straight line is composed of cells of size given by 'cellSize' and spaced by 'spacing'
// Each cell is composed of a point and its HOG data (i.e. descriptor values)
vector<vector<shared_ptr<Cell>>> MovementDetector::getFieldOfView(const Rect &target, const Mat &firstFrame,
                                                                  const Mat &lastFrame, int degrees, int spacing,
                                                                  const Size &cellSize)
{
    Point2f previousPosition, nextPosition;
    getMovementData(target, firstFrame, lastFrame, previousPosition, nextPosition);
    return getFieldOfViewHelper(previousPosition, nextPosition, lastFrame, degrees, spacing, cellSize);
}

vector<vector<shared_ptr<Cell>>> MovementDetector::getFieldOfView(const Rect &targetBefore, const Rect &targetAfter,
                                                                  Mat frameAfter, int degrees, int spacing,
                                                                  const Size &cellSize)
{
    Point2f previousPosition = Point((targetBefore.tl().x + targetBefore.br().x) / 2.0,
                                     (targetBefore.tl().y + targetBefore.br().y) / 2.0);
    
    Point2f nextPosition = Point((targetAfter.tl().x + targetAfter.br().x) / 2.0,
                                 (targetAfter.tl().y + targetAfter.br().y) / 2.0);
    
    return getFieldOfViewHelper(previousPosition, nextPosition, frameAfter, degrees, spacing, cellSize);
}

vector<vector<shared_ptr<Cell>>> MovementDetector::getFieldOfViewHelper(Point2f previousPosition, Point2f nextPosition,
                                                                        Mat frameAfter, int degrees, int spacing,
                                                                        const Size &cellSize)
{
    int direction = (nextPosition.y >= previousPosition.y) ? 1 : -1;
    double slope = (nextPosition.y - previousPosition.y) / (nextPosition.x - previousPosition.x);
    double angle = atan(-1/slope);
    
    Rect frameRect(Point(), frameAfter.size()); // Rect that matches the frame's size
    vector<vector<shared_ptr<Cell>>> fieldOfView;
    
    int howManyLines = degrees;
    if (degrees > 180) degrees -= 180;
    
    for (int i = 0; i < howManyLines; i++) {
        vector<shared_ptr<Cell>> straightLine;
        int distanceFromOrigin = 0;
        
        int j = 0;
        while (1) {
            distanceFromOrigin += spacing;
            
            Point currentPoint;
            currentPoint.x = round(nextPosition.x + distanceFromOrigin*cos(angle) * direction);
            currentPoint.y = round(nextPosition.y + distanceFromOrigin*sin(angle) * direction);
            
            // if the cell is not contained in the frame, it means the current line is finished
            if (!Utils::contains(frameRect, Rect(currentPoint, cellSize))) break;
            
            Point p;
            p.x = currentPoint.x - (cellSize.width / 2);
            p.y = currentPoint.y - (cellSize.height / 2);
            if (p.x < 0 || p.y < 0) break;
            
            shared_ptr<Cell> newCell = shared_ptr<Cell>(new Cell(currentPoint, computeHOG(currentPoint,
                                                                                          frameAfter, cellSize)));
            straightLine.push_back(newCell);
            j++;
        }
        
        fieldOfView.push_back(straightLine);
        angle += M_PI/degrees;
    }
    
    return fieldOfView;
}

vector<float> MovementDetector::computeHOG(const Point &point, const Mat &frame, const Size &cellSize)
{
    HOGDescriptor hog(cellSize, cellSize, Size(8,8), Size(8,8), 9); // maybe change this in the future?
    
    Point p;
    p.x = point.x - (cellSize.width / 2);
    p.y = point.y - (cellSize.height / 2);
    
    Mat cellImage(frame, Rect(p, cellSize));
    vector<float> descriptorValues;
    hog.compute(cellImage, descriptorValues);
    
    return descriptorValues;
}

// Returns 2 points: one determining the target's origin and the other pointing to where the target was moved to
void MovementDetector::getMovementData(const Rect &target, const Mat &firstFrame, const Mat &lastFrame,
                                       Point2f &previousPosition, Point2f &nextPosition)
{
    
    // Cropping both frames to obtain just the target (and then converting its color space to grayscale)
    Mat previousTarget, nextTarget;
    Mat(firstFrame, target).copyTo(previousTarget);
    Mat(lastFrame, target).copyTo(nextTarget);
    cvtColor(previousTarget, previousTarget, CV_BGR2GRAY);
    cvtColor(nextTarget, nextTarget, CV_BGR2GRAY);
    
    // Obtaining the target's features
    vector<Point2f> features;
    goodFeaturesToTrack(previousTarget, features, 20, 0.01, 1);
    
    // Computing the optical flow based on the target's features
    vector<Point2f> nextPoints = vector<Point2f>(features.size());
    vector<uchar> status;
    Mat err;
    calcOpticalFlowPyrLK(previousTarget, nextTarget, features, nextPoints, status, err);
    
    // Initializing the output points to (0,0)
    previousPosition = Point2f(0,0);
    nextPosition = Point2f(0,0);
    
    // Accumulating the points obtained goodFeaturesToTrack and calcOpticalFlowPyrLK
    int i = 0;
    for (Point2f feature : features) {
        previousPosition += feature;
        nextPosition += nextPoints[i++];
    }
    
    // Taking the average of the points and updating their coordinates to their position on frame
    previousPosition = previousPosition * (1.0/features.size());
    previousPosition = previousPosition + Point2f(target.x, target.y);
    nextPosition = nextPosition * (1.0/features.size());
    nextPosition = nextPosition + Point2f(target.x, target.y);
}
