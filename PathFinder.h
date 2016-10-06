//
//  PathFinder.h
//  BackgroundSubtraction
//
//  Created by Arthur Emídio Teixeira Ferreira on 9/7/15.
//  Copyright (c) 2015 Arthur Emídio Teixeira Ferreira. All rights reserved.
//

#ifndef __BackgroundSubtraction__PathFinder__
#define __BackgroundSubtraction__PathFinder__

#include "opencv2/opencv.hpp"
#include "Cell.h"
#include "Graph.h"

using namespace std;
using namespace cv;

enum ShortestPathAlgorithm {
    A_STAR,
    D_STAR,
    DIJKSTRA
};

class PathFinder {
public:
    /*!
     * \brief The field of view.
     */
    vector<vector<shared_ptr<Cell>>> fieldOfView;
    
    /*!
     * \brief Default constructor.
     * \param fieldOfView The field of view the \c PathFinder should be initialized with.
     */
    PathFinder(vector<vector<shared_ptr<Cell>>> fieldOfView);
    
    /*!
     * \brief Draws the shortest path in the graph from \c start to \goal.
     * \param img The image which would be drawn.
     * \param graph The graph.
     * \param goal The goal point.
     * \param start The start point.
     * \param color The line color to be drawn.
     * \param algorithm The shortest-path algorithm to be used.
     * \param path The set of points indicating the drawn path.
     */
    void drawShortestPaths(Mat img, Graph &graph, const Point &goal, const Point &start, Scalar color,
                           ShortestPathAlgorithm algorithm, vector<Point> &path);
    
    void drawNaivePaths(Mat img, int n, Scalar color);
    
    void drawOptimalCells(Mat img, int score, Scalar color);
    
private:
    /*!
     * \brief Checks whether a point exists in the field of view.
     * \param The point's row.
     * \param The point's column
     * \return \c true if the point exists in the field of view, or \c false otherwise.
     */
    bool doesPointExist(int row, int col) const;
    
    /*!
     * \brief Computes the resulting HOG score for each cell in the field of view.
     *
     * Every cell gets 
     */
    void calculateScores();
    
    /*!
     * \brief Computes the HOG difference between the cell at the given coordinates and all other cells in its 8-neighborhood
     * \param row The cell's row.
     * \param col The cell's column.
     * \param avgDiff[out] The average HOG differences between the cell at (row, column) and its 8-neighborhood.
     * \return A vector of pairs, containing as first element each cell C in its 8-neighborhood, and the HOG difference between
     * the cell at (row, column) and the cell C as second element.
     */
    vector<pair<Point, double>> computeNeighborhoodDiffence(int row, int col, double &avgDiff);
    
    vector<pair<vector<shared_ptr<Cell>>, double>> findNaiveOptimalPaths();
    
    static bool compareNaiveScores(pair<vector<shared_ptr<Cell>>, double> pair1, pair<vector<shared_ptr<Cell>>, double> pair2);

};

#endif
