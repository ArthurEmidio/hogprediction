//
//  Filter.cpp
//  BackgroundSubtraction
//
//  Created by Arthur Emídio Teixeira Ferreira on 3/6/15.
//  Copyright (c) 2015 Arthur Emídio Teixeira Ferreira. All rights reserved.
//

#include "Filter.h"

/* Public Methods */

Filter::Filter(Mat img, vector<vector<Point>> contours, int C) {
    
    this->image = img;
    this->contours = contours;
    this->CONSTANT = C;
    
    double smallestArea, biggestArea;
    obtainAreas(&smallestArea, &biggestArea);
    obtainScores(smallestArea, biggestArea);
    sortPairsByScore();
    classifyContours();
}

vector<vector<Point>> Filter::getAllContours() {
    return contours;
}

vector<vector<Point>> Filter::getValidContours() {
    return validContours;
}

vector<vector<Point>> Filter::getInvalidContours() {
    return invalidContours;
}


// Private Methods
void Filter::obtainAreas(double *smallestArea, double *biggestArea) {
    
    int biggestAreaIndex = 0;
    int smallestAreaIndex = 0;
    
    *biggestArea = boundingRect(contours.at(biggestAreaIndex)).area();
    *smallestArea = boundingRect(contours.at(smallestAreaIndex)).area();
    
    double currArea;
    for(int i = 1; i < contours.size(); i++) {
        currArea = boundingRect(contours.at(i)).area();
        
        if(currArea > *biggestArea) {
            biggestAreaIndex = i;
            *biggestArea = currArea;
        }
        
        if(currArea < *smallestArea) {
            smallestAreaIndex = i;
            *smallestArea = currArea;
        }
    }
    
}

void Filter::obtainScores(double smallestArea, double biggestArea) {
    
    double areaGap = biggestArea - smallestArea;
    double factor;
    
    for(int i = 0; i < contours.size(); i++) {
        factor = CONSTANT * abs((boundingRect(contours.at(i)).area() - smallestArea)/areaGap);
        pair<vector<Point>, double> pair(contours[i], factor);
        pairs.push_back(pair);
    }
    
}

bool Filter::compareScores(pair<vector<Point>, double> pair1, pair<vector<Point>, double> pair2) {
    return pair1.second > pair2.second;
}

void Filter::sortPairsByScore() {
    std::sort(pairs.begin(), pairs.end(), Filter::compareScores);
}

void Filter::classifyContours() {
    
    double greatestGap = 0; // threshold (talvez mudar isso)
    int greatestGapIndex = 0;
    double gap;
    
    for (int i = 1; i < pairs.size(); i++) {
        gap = pairs.at(i-1).second - pairs.at(i).second;
        if (gap > greatestGap) {
            greatestGap = gap;
            greatestGapIndex = i;
        }
    }
    
    int i = 0;
    while (i < greatestGapIndex) {
        validContours.push_back(pairs.at(i).first);
        i++;
    }
    
    while (i < pairs.size()) {
        invalidContours.push_back(pairs.at(i).first);
        i++;
    }
}
