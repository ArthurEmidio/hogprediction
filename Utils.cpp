//
//  Utils.cpp
//  BackgroundSubtraction
//
//  Created by Arthur Emídio Teixeira Ferreira on 3/10/15.
//  Copyright (c) 2015 Arthur Emídio Teixeira Ferreira. All rights reserved.
//

#include "Utils.h"
#include <Python/Python.h>
#include <fstream>

vector<Rect> Utils::getRectanglesFromContours(vector<vector<Point>> contours)
{
    vector<Rect> rectangles;
    
    for (int i = 0; i < contours.size(); i++) {
        rectangles.push_back(boundingRect(contours[i]));
    }
    
    return rectangles;
}

bool Utils::contains(Rect container, Rect contained)
{
    Point upperLeft(contained.x, contained.y);
    Point upperRight(contained.x + contained.width, contained.y);
    Point bottomRight(contained.x + contained.width, contained.y + contained.height);
    Point bottomLeft(contained.x, contained.y + contained.height);
    
    if (container.contains(upperLeft) && container.contains(upperRight) &&
       container.contains(bottomRight) && container.contains(bottomLeft)) {
        return true;
    }
    
    return false;
}

vector<double> Utils::subtractVectors(vector<float> v1, vector<float> v2)
{
    if (v1.size() != v2.size()) {
        cout << "Error: vector with different sizes.";
        exit(1);
    }
    
    unsigned long s = v1.size();
    vector<double> res;
    for (int i = 0; i < s; i++) {
        res.push_back(abs(v1[i] - v2[i]));
    }
    
    return res;
}

void Utils::drawFieldOfView(vector<vector<shared_ptr<Cell>>> fieldOfView, Mat img, Scalar color)
{
    int i = 0;
    for(const vector<shared_ptr<Cell>> &cells : fieldOfView) {
        int j = 0;
        for(shared_ptr<Cell> c : cells) {
            circle(img, c->position, 3, color, -1);
            j++;
        }
        i++;
    }
}

void Utils::drawGraph(vector<Point2f> points, string xLabel, string yLabel)
{
    string xArg;
    string yArg;
    for (const Point &point : points) {
        xArg.append(to_string(point.x) + ",");
        yArg.append(to_string(point.y) + ",");
    }
    xArg.erase(xArg.end() - 1, xArg.end());
    yArg.erase(yArg.end() - 1, yArg.end());
    
    string pythonProgram = "import matplotlib.pyplot as plt\n"
                           "plt.plot(["+ xArg + "], [" + yArg + "])\n"
                           "plt.ylabel('" + yLabel + "')\n"
                           "plt.xlabel('" + xLabel + "')\n"
                           "plt.show()\n";
    
    Py_SetProgramName((char *) "plot");
    Py_Initialize();
    PyRun_SimpleString(pythonProgram.c_str());
    Py_Finalize();
}

double Utils::distFromNearestPoint(Point point, vector<Point> groundTruth, Point &nearestPoint)
{
    double minDist = numeric_limits<double>::max();
    for (const Point &gtPoint : groundTruth) {
        double dist = hypot(gtPoint.x - point.x, gtPoint.y - point.y);
        if (dist < minDist) {
            minDist = dist;
            nearestPoint = gtPoint;
        }
    }
    
    return minDist;
}

void Utils::savePointsToCsv(vector<Point2f> points, string path, string fileName)
{
    ofstream file;
    file.open(path + "/" + fileName);
    for (const Point2f &point : points) {
        file << to_string(point.x) + "," + to_string(point.y) + "\n";
    }
    file.close();
}

// code from: http://www.juergenwiki.de/work/wiki/doku.php?id=public%3ahog_descriptor_computation_and_visualization
Mat Utils::get_hogdescriptor_visual_image(Mat& origImg,
                                   vector<float>& descriptorValues,
                                   Size winSize,
                                   Size cellSize,
                                   int scaleFactor,
                                   double viz_factor, Mat &visual_image)
{
    //Mat visual_image;
    //resize(origImg, visual_image, Size(origImg.cols*scaleFactor, origImg.rows*scaleFactor));
    
    int gradientBinSize = 9;
    // dividing 180° into 9 bins, how large (in rad) is one bin?
    float radRangeForOneBin = 3.14/(float)gradientBinSize;
    
    // prepare data structure: 9 orientation / gradient strenghts for each cell
    int cells_in_x_dir = winSize.width / cellSize.width;
    int cells_in_y_dir = winSize.height / cellSize.height;
    float*** gradientStrengths = new float**[cells_in_y_dir];
    int** cellUpdateCounter   = new int*[cells_in_y_dir];
    for (int y=0; y<cells_in_y_dir; y++)
    {
        gradientStrengths[y] = new float*[cells_in_x_dir];
        cellUpdateCounter[y] = new int[cells_in_x_dir];
        for (int x=0; x<cells_in_x_dir; x++)
        {
            gradientStrengths[y][x] = new float[gradientBinSize];
            cellUpdateCounter[y][x] = 0;
            
            for (int bin=0; bin<gradientBinSize; bin++)
                gradientStrengths[y][x][bin] = 0.0;
        }
    }
    
    // nr of blocks = nr of cells - 1
    // since there is a new block on each cell (overlapping blocks!) but the last one
    int blocks_in_x_dir = cells_in_x_dir - 1;
    int blocks_in_y_dir = cells_in_y_dir - 1;
    
    // compute gradient strengths per cell
    int descriptorDataIdx = 0;
    
    for (int blockx=0; blockx<blocks_in_x_dir; blockx++)
    {
        for (int blocky=0; blocky<blocks_in_y_dir; blocky++)
        {
            // 4 cells per block ...
            for (int cellNr=0; cellNr<4; cellNr++)
            {
                // compute corresponding cell nr
                int cellx = blockx;
                int celly = blocky;
                if (cellNr==1) celly++;
                if (cellNr==2) cellx++;
                if (cellNr==3)
                {
                    cellx++;
                    celly++;
                }
                
                for (int bin=0; bin<gradientBinSize; bin++)
                {
                    float gradientStrength = descriptorValues[ descriptorDataIdx ];
                    descriptorDataIdx++;
                    
                    gradientStrengths[celly][cellx][bin] += gradientStrength;
                    
                } // for (all bins)
                
                
                // note: overlapping blocks lead to multiple updates of this sum!
                // we therefore keep track how often a cell was updated,
                // to compute average gradient strengths
                cellUpdateCounter[celly][cellx]++;
                
            } // for (all cells)
            
            
        } // for (all block x pos)
    } // for (all block y pos)
    
    
    // compute average gradient strengths
    for (int celly=0; celly<cells_in_y_dir; celly++)
    {
        for (int cellx=0; cellx<cells_in_x_dir; cellx++)
        {
            
            float NrUpdatesForThisCell = (float)cellUpdateCounter[celly][cellx];
            
            // compute average gradient strenghts for each gradient bin direction
            for (int bin=0; bin<gradientBinSize; bin++)
            {
                gradientStrengths[celly][cellx][bin] /= NrUpdatesForThisCell;
            }
        }
    }
    
    
    //cout << "descriptorDataIdx = " << descriptorDataIdx << endl;
    
    // draw cells
    for (int celly=0; celly<cells_in_y_dir; celly++)
    {
        for (int cellx=0; cellx<cells_in_x_dir; cellx++)
        {
            int drawX = cellx * cellSize.width;
            int drawY = celly * cellSize.height;
            
            int mx = drawX + cellSize.width/2;
            int my = drawY + cellSize.height/2;
            
            rectangle(visual_image,
                      Point(drawX*scaleFactor,drawY*scaleFactor),
                      Point((drawX+cellSize.width)*scaleFactor,
                            (drawY+cellSize.height)*scaleFactor),
                      CV_RGB(100,100,100),
                      1);
            
            // draw in each cell all 9 gradient strengths
            for (int bin=0; bin<gradientBinSize; bin++)
            {
                float currentGradStrength = gradientStrengths[celly][cellx][bin];
                
                // no line to draw?
                if (currentGradStrength==0)
                    continue;
                
                float currRad = bin * radRangeForOneBin + radRangeForOneBin/2;
                
                float dirVecX = cos( currRad );
                float dirVecY = sin( currRad );
                float maxVecLen = cellSize.width/2;
                float scale = viz_factor; // just a visual_imagealization scale,
                // to see the lines better
                
                // compute line coordinates
                float x1 = mx - dirVecX * currentGradStrength * maxVecLen * scale;
                float y1 = my - dirVecY * currentGradStrength * maxVecLen * scale;
                float x2 = mx + dirVecX * currentGradStrength * maxVecLen * scale;
                float y2 = my + dirVecY * currentGradStrength * maxVecLen * scale;
                
                // draw gradient visual_imagealization
                line(visual_image,
                     Point(x1*scaleFactor,y1*scaleFactor),
                     Point(x2*scaleFactor,y2*scaleFactor),
                     CV_RGB(0,255,0),
                     1);
                
            } // for (all bins)
            
        } // for (cellx)
    } // for (celly)
    
    
    // don't forget to free memory allocated by helper data structures!
    for (int y=0; y<cells_in_y_dir; y++)
    {
        for (int x=0; x<cells_in_x_dir; x++)
        {
            delete[] gradientStrengths[y][x];
        }
        delete[] gradientStrengths[y];
        delete[] cellUpdateCounter[y];
    }
    delete[] gradientStrengths;
    delete[] cellUpdateCounter;
    
    return visual_image;
}
