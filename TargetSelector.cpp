//
//  TargetSelector.cpp
//  BackgroundSubtraction
//
//  Created by Arthur Emídio Teixeira Ferreira on 1/6/16.
//  Copyright © 2016 Arthur Emídio Teixeira Ferreira. All rights reserved.
//

#include "TargetSelector.h"

class TargetPack
{
public:
    Mat original;
    Mat copy;
    Mat resultingTarget;
    Point points[2];
    int howManyPoints;
    
    TargetPack(const Mat &mat) {
        original = mat;
        original.copyTo(copy);
        howManyPoints = 0;
    }
};

void TargetSelector::onMouse(int event, int x, int y, int, void *param)
{
    TargetPack *targetPack = (TargetPack*) param;
    
    if (event == EVENT_LBUTTONDOWN && targetPack->howManyPoints < 2) {
        targetPack->points[targetPack->howManyPoints] = Point(x, y);
        targetPack->howManyPoints++;

        if (targetPack->howManyPoints == 2) {
            targetPack->resultingTarget = Mat(targetPack->original,
                                              Rect(min(targetPack->points[0].x, targetPack->points[1].x),
                                                   min(targetPack->points[0].y, targetPack->points[1].y),
                                                   abs(targetPack->points[0].x - targetPack->points[1].x),
                                                   abs(targetPack->points[0].y - targetPack->points[1].y)));
        }
    } else if (event == EVENT_MOUSEMOVE && targetPack->howManyPoints == 1) {
        double alpha = 0.60;
        targetPack->original.copyTo(targetPack->copy);
        Mat roi = Mat(targetPack->copy, Rect(targetPack->points[0], Point(x,y)));
        Mat colorMask = Mat(roi.size(), CV_8UC3, Scalar(69, 52, 219));
        addWeighted(roi, alpha, colorMask, 1.0 - alpha, 0.0, roi);
    }
}

TargetSelector::TargetSelector(const Mat &img) : _originalImage(img)
{
    TargetPack targetPack = TargetPack(img);
    
    namedWindow("Select Target");
    setMouseCallback("Select Target", onMouse, &targetPack);
    
    while (targetPack.resultingTarget.elemSize() == 0) {
        imshow("Select Target", targetPack.copy);
        waitKey(1);
    }
    targetPack.resultingTarget.copyTo(_target);
    _target.copyTo(_initialTarget);
    
    destroyWindow("Select Target");
}

TargetSelector::TargetSelector(const Mat &img, const Rect &target)
{
    Mat originalCopy;
    img.copyTo(originalCopy);
    _target = Mat(originalCopy, target);
    _target.copyTo(_initialTarget);
}

void TargetSelector::reset() {
    _initialTarget.copyTo(_target);
}

Mat TargetSelector::getTargetMat() {
    return _target;
}

// Locates the target at the given image using template matching (using the method "SQDIFF_NORMED").
// A rectangle is returned, which corresponds to where the target is located in the given image.
Rect TargetSelector::locateTarget(const Mat &img)
{
    assert(_target.elemSize() > 0);
    
    Mat result = Mat(img.rows - _target.rows + 1, img.cols - _target.cols + 1, CV_32FC1);
    matchTemplate(img, _target, result, CV_TM_SQDIFF_NORMED);
    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
    
    double minVal, maxVal;
    Point minLoc, maxLoc;
    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
    Point matchLoc = minLoc; // the best match for the SQDIFF_NORMED method is the one with minimum values
    
    Rect rectMatch = Rect(matchLoc, Point(matchLoc.x + _target.cols, matchLoc.y + _target.rows));
    _target = Mat(img, rectMatch); // updating the target
    
    return rectMatch;
}
