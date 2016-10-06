#Vehicle Path Prediction Using HOG

This repository contains the implementation of an undergraduate research project
developed with the goal to find trajectories that a vehicle is likely to traverse
in a scene using [Histogram of Oriented Gradients (HOG)](https://en.wikipedia.org/wiki/Histogram_of_oriented_gradients) 
as main feature.

##Usage
Insert all frames (as .jpg images) of the input video in a folder named "input" in the same directory where
the binary is located. The frames must be numbered as: "i.jpg", where i = 0, 1, ..., N.

It is important to note that the frames should always contain the vehicle which will have the trajectory predicted.

Run the executable, select the vehicle of interest. The trajectory prediction based on the first frame will be shown.
Press any key after each iteration to visualize the next predicted trajectory.

If you're looking for a dataset of aerial videos, the [MINSK dataset](http://www.tft.lth.se/en/research/video-analysis/co-operation/data-exchange/) (Saunier et al.) is a great option.

##Dependencies
1. OpenCV 2.4.11
2. Python 2.7

##License
MIT License

Copyright (c) 2016 Arthur Emídio Teixeira Ferreira

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.