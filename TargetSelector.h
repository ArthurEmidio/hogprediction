//
//  TargetSelector.h
//  BackgroundSubtraction
//
//  Created by Arthur Emídio Teixeira Ferreira on 1/6/16.
//  Copyright © 2016 Arthur Emídio Teixeira Ferreira. All rights reserved.
//

#ifndef TargetSelector_h
#define TargetSelector_h

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

class TargetSelector {
public:
    TargetSelector(const Mat &img);
    
    TargetSelector(const Mat &img, const Rect &target);
    
    Rect locateTarget(const Mat &img);
    
    Mat getTargetMat();
    
    void reset();
   
private:
    static void onMouse(int event, int x, int y, int, void *param);
    
    Mat _originalImage;
    Mat _target;
    Mat _initialTarget;
    
};



#endif /* TargetSelector_h */
