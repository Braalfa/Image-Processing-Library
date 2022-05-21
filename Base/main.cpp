#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;

int findMedian(int array[], int size){
    std::sort(array,array+size);
    return array[size/2];
}

void medianFilter(Mat image){
    int windowSize = 9;
    int window[windowSize];    
    Mat filteredImage = image.clone();
    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
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
    imwrite("medianFilterOutput.jpg", filteredImage);
}

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
    return C;
}

void printMat(Mat mat){
    for(int j = 0; j < mat.rows; j++){
        for(int k = 0; k < mat.cols; k++){
            cout << mat.at<double>(j,k) << ' ';
        }
        cout << endl;
    }
}

void averageFilter(Mat image){

    image.convertTo(image, CV_64F);
    vector<Mat> channels(3);
    split(image, channels);
    int k = 3;
    cv::Mat matB(k, k, CV_64F, 1/(pow(k,2)));
    for(int i = 0; i< 20; i++){
        channels[0] = convolution2D(channels[0], matB);
        channels[1] = convolution2D(channels[1], matB);
        channels[2] = convolution2D(channels[2], matB);
    }
    cv::Mat final = cv::Mat(channels[0].rows, channels[0].cols, CV_64F);
    merge(channels, final);
    imwrite("averageConvolutionalFilterOutput.jpg", final);
}

int main(int argc, char** argv)
{
    Mat image = imread("barbara.jpg",
                       IMREAD_COLOR);

    if (image.empty()) {
        cout << "Image File "
             << "Not Found" << endl;
        cin.get();
        return -1;
    }

    //medianFilter(image);
    averageFilter(image);    
    return 0;
}
