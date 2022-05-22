#include "baseFilters.hpp"

int findMedian(int array[], int size){
    std::sort(array,array+size);
    return array[size/2];
}

Mat medianFilter(Mat* imagePtr){
    Mat image = *imagePtr;
    int windowSize = 9;
    int window[windowSize];    
    Mat filteredImage = image.clone();
    for(int i = 1; i < image.rows-1; i++)
    {
        for(int j = 1; j < image.cols-1; j++)
        {
            Vec3b newPixel;
            for(int k = 0; k < 3; k++){
                window[0] = image.at<Vec3b>(i-1, j-1).val[k];
                window[1] = image.at<Vec3b>(i, j-1).val[k];
                window[2] = image.at<Vec3b>(i+1, j-1).val[k];
                window[3] = image.at<Vec3b>(i-1, j).val[k];
                window[4] = image.at<Vec3b>(i, j).val[k];
                window[5] = image.at<Vec3b>(i+1, j).val[k];
                window[6] = image.at<Vec3b>(i-1, j+1).val[k];
                window[7] = image.at<Vec3b>(i, j+1).val[k];
                window[8] = image.at<Vec3b>(i+1, j+1).val[k];
                newPixel.val[k] = findMedian(window, windowSize);
            } 
            filteredImage.at<Vec3b>(i, j) = newPixel;
        }
    }
    return filteredImage;
}