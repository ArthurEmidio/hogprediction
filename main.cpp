#include <opencv2/opencv.hpp>
#include <Python/Python.h>

#include "Detector.h"
#include "Utils.h"
#include "MovementDetector.h"
#include "Cell.h"
#include "PathFinder.h"
#include "Graph.h"
#include "TargetSelector.h"

using namespace cv;
using namespace std;

const string folder = "input";

inline Mat getImage(int index, string ext) {
    return imread(folder + "/" + to_string(index) + "." + "jpg");
}

int main(int argc, const char *argv[])
{
    TargetSelector targetSelector(getImage(0, "jpg"));
    int maxIndex = 1;
    Rect currTarget;
    for (Mat currImage = getImage(maxIndex, "jpg"); !currImage.empty(); currImage = getImage(++maxIndex, "jpg")) {
        currTarget = targetSelector.locateTarget(currImage);
    }
    Point targetFinalPosition = Point(currTarget.tl().x + (currTarget.width / 2.0),
                                      currTarget.tl().y + (currTarget.height / 2.0));
    
    targetSelector.reset();
    Rect prevTarget = targetSelector.locateTarget(getImage(0, "jpg"));
    Rect prevPrevTarget = prevTarget;
    
    int imageIndex = 5;
    int iterationIndex = 0;
    Mat currImage;
    
    Mat firstFrame = getImage(1, "jpg");
    for (currImage = getImage(imageIndex, "jpg"); imageIndex < maxIndex; currImage = getImage(imageIndex, "jpg")) {
        Mat imgDraw;
        currImage.copyTo(imgDraw);
        
        currTarget = targetSelector.locateTarget(currImage);
        vector<vector<shared_ptr<Cell>>> fieldOfView = MovementDetector::getFieldOfView(prevTarget, currTarget,
                                                                                        currImage, 180, 8,
                                                                                        Size(16,16));
        
        if (!fieldOfView.at(90).empty()) {
            prevPrevTarget = prevTarget;
            prevTarget = currTarget;
        } else if (prevTarget != prevPrevTarget) {
            fieldOfView = MovementDetector::getFieldOfView(prevPrevTarget, prevTarget,
                                                           currImage, 180, 8,
                                                           Size(16,16));
        } else {
            break;
        }
        
        if (fieldOfView.at(90).size() < 3) {
            break;
        }
        Point startPosition = fieldOfView.at(90).at(2)->position;
        
        Graph graph(fieldOfView);
        PathFinder pathFinder(fieldOfView);
        
        vector<Point> predictedPath;
        pathFinder.drawShortestPaths(imgDraw, graph, targetFinalPosition, startPosition, Scalar(0, 0, 255),
                                     ShortestPathAlgorithm::A_STAR, predictedPath);
        
        imshow("Target", imgDraw);
        waitKey(1);
        
        iterationIndex++;
        imageIndex += 5;
    }
    
    destroyAllWindows();
    
    return 0;
}
