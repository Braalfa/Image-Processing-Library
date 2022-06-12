#include "optimizedFilters.hpp"

int findMaxInMatRange(Mat mat, int xmin, int xmax, int ymin, int ymax){
    double maxval = -INFINITY;
    for(int x=xmin; x<xmax;x++){
        for(int y=ymin; y<ymax;y++){
            if (mat.at<double>(x,y) > maxval){
                maxval = mat.at<double>(x,y);
            }
        }
    }
    return maxval;
}

int findMinInMatRange(Mat mat, int xmin, int xmax, int ymin, int ymax){
    double minval = INFINITY;
    for(int x=xmin; x<xmax;x++){
        for(int y=ymin; y<ymax;y++){
            if (mat.at<double>(x,y) < minval){
                minval = mat.at<double>(x,y);
            }
        }
    }
    return minval;
}

Mat dilatation(Mat* imagePtr){
    Mat image = *imagePtr;
    int m = image.rows;
    int n = image.cols;
    cv::Mat out(m, n, CV_64F, 0.0);
    #pragma omp parallel for
    for(int x=0; x<m; x++){
        int xmin = max(x-1,0);
        int xmax = min(x+2,m);
        for(int y=0; y<n; y++){
            int ymin = max(y-1,0);
            int ymax = min(y+2,n);
            out.at<double>(x,y)= findMaxInMatRange(image, xmin, xmax, ymin, ymax);
        }
    }
    return out;
}

Mat erotion(Mat* imagePtr){
    Mat image = *imagePtr;
    int m = image.rows;
    int n = image.cols;
    cv::Mat out(m, n, CV_64F, 0.0);
    #pragma omp parallel for
    for(int x=0; x<m; x++){
        int xmin = max(x-1,0);
        int xmax = min(x+2,m);
        for(int y=0; y<n; y++){
            int ymin = max(y-1,0);
            int ymax = min(y+2,n);
            out.at<double>(x,y)= findMinInMatRange(image, xmin, xmax, ymin, ymax);
        }
    }
    return out;
}

Mat open(Mat* imagePtr){
    Mat tmp = erotion(imagePtr);
    Mat out = dilatation(&tmp);
    return out;
}

Mat clausure(Mat* imagePtr){
    Mat tmp = dilatation(imagePtr);
    Mat out = erotion(&tmp);
    return out;
}

Mat topHatFilter(Mat* imagePtr){
    Mat image = *imagePtr;
    Mat out = open(imagePtr);
    #pragma omp parallel for collapse(2)
    for(int i = 0; i <image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            out.at<double>(i, j) = image.at<double>(i, j) - out.at<double>(i, j);
        }
    }
    return out;
}

Mat bottomHatFilter(Mat* imagePtr){
    Mat image = *imagePtr;
    Mat out = clausure(imagePtr);
    #pragma omp parallel for collapse(2)
    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            out.at<double>(i, j) = out.at<double>(i, j) -  image.at<double>(i, j);
        }
    }
    return out;
}

Mat contrastEnhancementFilter(Mat* imagePtr){
    Mat image = *imagePtr;
    Mat topHatImage = topHatFilter(imagePtr);
    Mat bottomHatImage = bottomHatFilter(imagePtr);
    cv::Mat out(image.rows, image.cols, CV_64F, 0.0);
    #pragma omp parallel for collapse(2)
    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            out.at<double>(i, j) = image.at<double>(i, j) +  topHatImage.at<double>(i, j) - bottomHatImage.at<double>(i, j);
        }
    }
    return out;
}