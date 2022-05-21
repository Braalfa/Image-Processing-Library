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

Mat medianFilter(Mat* imagePtr){
    Mat image = *imagePtr;
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
    return filteredImage;
}

// Mat convolution2D(Mat A, Mat B){

//     int m1 = A.rows;
//     int n1 = A.cols;

//     int m2 = B.rows;
//     int n2 = B.cols;

//     int nRows = m1+m2-1;
//     int nCols = n1+n2-1;

//     Mat C = Mat::zeros(nRows, nCols, CV_64F);

//     for(int j = 0; j < nRows; j++){
//         for(int k = 0; k < nCols; k++){
//             for(int p = max(0, j-m2+1); p < min(j+1, m1); p++){
//                 for(int q = max(0, k-n2+1); q < min(k+1, n1); q++){
//                      C.at<double>(j,k)=C.at<double>(j,k)+A.at<double>(p,q)*B.at<double>(j-p,k-q);
//                 }
//             }
//         }
//     }
//     return C;
// }

Mat convolution2D(Mat A, Mat B){

    int m1 = A.rows;
    int n1 = A.cols;

    int m2 = B.rows;
    int n2 = B.cols;

    int nRows = m1;
    int nCols = n1;

    Mat C = Mat::zeros(nRows, nCols, CV_64F);
    // Convolution
    for(int j = 0; j < nRows; j++){
        for(int k = 0; k < nCols; k++){
            for(int p = max(0, j-m2+1); p < min(j+1, m1); p++){
                for(int q = max(0, k-n2+1); q < min(k+1, n1); q++){
                     C.at<double>(j+m2/2-1,k+n2/2-1)=C.at<double>(j+m2/2-1,k+n2/2-1)+A.at<double>(p,q)*B.at<double>(j-p,k-q);
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

Mat averageFilter(Mat* imagePtr, int iterations){
    Mat image = *imagePtr;
    vector<Mat> channels(3);
    split(image, channels);
    int k = 3;
    cv::Mat matB(k, k, CV_64F, 1/(pow(k,2)));
    for(int i = 0; i< iterations; i++){
        channels[0] = convolution2D(channels[0], matB);
        channels[1] = convolution2D(channels[1], matB);
        channels[2] = convolution2D(channels[2], matB);
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
    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            matC.at<double>(i, j) = image.at<double>(i, j) +  matB.at<double>(i, j)*alpha;
        }
    }

    return matC;
}

int findMaxInMatRange(Mat mat, int xmin, int xmax, int ymin, int ymax){
    double max = -INFINITY;
    for(int x=xmin; x<xmax;x++){
        for(int y=ymin; y<ymax;y++){
            if (mat.at<double>(x,y) > max){
                max = mat.at<double>(x,y);
            }
        }
    }
    return max;
}

int findMinInMatRange(Mat mat, int xmin, int xmax, int ymin, int ymax){
    double min = INFINITY;
    for(int x=xmin; x<xmax;x++){
        for(int y=ymin; y<ymax;y++){
            if (mat.at<double>(x,y) < min){
                min = mat.at<double>(x,y);
            }
        }
    }
    return min;
}

Mat dilatation(Mat* imagePtr){
    Mat image = *imagePtr;
    int m = image.rows;
    int n = image.cols;
    cv::Mat out(m, n, CV_64F, 0.0);

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

Mat topHat(Mat* imagePtr){
    Mat image = *imagePtr;
    Mat out = open(imagePtr);
    for(int i = 0; i <image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            out.at<double>(i, j) = image.at<double>(i, j) - out.at<double>(i, j);
        }
    }
    imwrite("top.jpg", out);
    return out;
}

Mat bottomHat(Mat* imagePtr){
    Mat image = *imagePtr;
    Mat out = clausure(imagePtr);
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
    Mat topHatImage = topHat(imagePtr);
    Mat bottomHatImage = bottomHat(imagePtr);
    cv::Mat out(image.rows, image.cols, CV_64F, 0.0);
    for(int i = 0; i < image.rows; i++)
    {
        for(int j = 0; j < image.cols; j++)
        {
            out.at<double>(i, j) = image.at<double>(i, j) +  topHatImage.at<double>(i, j) - bottomHatImage.at<double>(i, j);
        }
    }
    return out;
}

int main(int argc, char** argv)
{
    //Mat image = imread("barbara.jpg",
    //                    IMREAD_COLOR);

    // Mat image = imread("imagen11.jpg",
    //                   IMREAD_GRAYSCALE);

    Mat image = imread("Columnas.jpg",
                   IMREAD_GRAYSCALE);


    if (image.empty()) {
        cout << "Image File "
             << "Not Found" << endl;
        cin.get();
        return -1;
    }
    image.convertTo(image, CV_64F);

    Mat* ptr = &image;
    //contrastEnhancementFilter(ptr);
    //image.convertTo(image, CV_8U);
    //Mat out = medianFilter(ptr);
    //Mat out =averageFilter(ptr, 1); 
    Mat out = bordersEnhancementFilter(ptr);   
    imwrite("output.jpg", out);
    return 0;
}
