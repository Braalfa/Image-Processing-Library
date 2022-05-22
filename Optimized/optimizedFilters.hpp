#ifndef OPTIMIZED_FILTERS_H_INCLUDED
#define OPTIMIZED_FILTERS_H_INCLUDED

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

Mat medianFilter(Mat* imagePtr);

Mat averageFilter(Mat* imagePtr, int iterations);
Mat laplacianFilter(Mat* imagePtr); // Aux filter for border enhancement
Mat bordersEnhancementFilter(Mat* imagePtr);

Mat topHatFilter(Mat* imagePtr); // Aux morphological filter
Mat bottomHatFilter(Mat* imagePtr); // Aux morphological filter
Mat contrastEnhancementFilter(Mat* imagePtr); // Main morphological filter

#endif
