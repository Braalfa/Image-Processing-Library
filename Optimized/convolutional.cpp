#include "optimizedFilters.hpp"
#include <iostream>

Mat convolution2D(Mat A, Mat B){

    int m1 = A.rows;
    int n1 = A.cols;

    int m2 = B.rows;
    int n2 = B.cols;

    int nRows = m1+m2-1;
    int nCols = n1+n2-1;

    Mat C = Mat::zeros(nRows, nCols, CV_64F);
    for(int j = 0; j < nRows; j++){
        for(int k = 0; k < nCols; k++){
            for(int p = max(0, j-m2+1); p < min(j+1, m1); p++){
                for(int q = max(0, k-n2+1); q < min(k+1, n1); q++){
                     C.at<double>(j,k)=C.at<double>(j,k)+A.at<double>(p,q)*B.at<double>(j-p,k-q);
                }
            }
        }
    }

    Mat D = Mat::zeros(m1, n1, CV_64F);
    #pragma omp parallel for collapse(2)
    for(int j = 0; j < m1; j++){
        for(int k = 0; k < n1; k++){
            D.at<double>(j,k)=C.at<double>(j+m2/2,k+n2/2);
        }
    }    
    return D;
}


Mat averageFilter(Mat* imagePtr, int iterations){
    Mat image = *imagePtr;
    vector<Mat> channels(3);
    split(image, channels);
    int k = 3;
    cv::Mat matB(k, k, CV_64F, 1/(pow(k,2)));
    
    for(int i = 0; i< iterations; i++){
        #pragma omp parallel for
        for(int j=0; j<3; j++){
            channels[j] = convolution2D(channels[j], matB);
        }
    }
    cv::Mat filteredImage = cv::Mat(channels[0].rows, channels[0].cols, CV_64F);
    merge(channels, filteredImage);
    return filteredImage;
}

Mat laplacianFilter(Mat* imagePtr){
    Mat image = *imagePtr;
    double B[3][3] = {{1, 1,1}, {1,-8,1}, {1, 1,1}};
    Mat matB(3, 3, CV_64F, B);
    Mat matC = convolution2D(image,matB);
    return matC;
}

Mat bordersEnhancementFilter(Mat* imagePtr){
    Mat image = *imagePtr;
    Mat matB = laplacianFilter(imagePtr);

    double alpha=-0.5;
    Mat matC(image.rows, image.cols, CV_64F, 0.0);
    #pragma omp for collapse(2)
    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            matC.at<double>(i, j) = image.at<double>(i, j) +  matB.at<double>(i, j)*alpha;
        }
    }

    return matC;
}