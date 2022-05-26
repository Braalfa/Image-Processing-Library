#include "Base/baseFilters.hpp"
#include <ctime>
#include <iostream>

int main(int argc, char** argv)
{
    Mat image;
    Mat* ptr;
    Mat out;

    //======================== MEDIAN TEST ========================

    // image = imread("InputImages/barbara.jpg",
    //                     IMREAD_COLOR);
    // ptr = &image;
    // for(int i = 0; i<10; i++){
    //     out = medianFilter(ptr);
    // }
    // imwrite("OutputImages/medianFilter.jpg", out);

    //=============================================================

    //======================== AVERAGE TEST ========================

    // image = imread("InputImages/barbara.jpg",
    //                     IMREAD_COLOR);
    // ptr = &image;
    // image.convertTo(image, CV_64F);
    // out = averageFilter(ptr, 30); 
    // imwrite("OutputImages/averageFilter.jpg", out);

    //=============================================================

    //================== BORDER ENHANCEMENT TEST ==================

    // image = imread("InputImages/Columnas.jpg",
    //                IMREAD_GRAYSCALE);
    // image.convertTo(image, CV_64F);
    // ptr = &image;

    // out = bordersEnhancementFilter(ptr);   
    // imwrite("OutputImages/borderEnhancement.jpg", out);

    //=============================================================

    //================= CONTRAST ENHANCEMENT TEST =================

    image = imread("InputImages/imagen11.jpg",
                    IMREAD_GRAYSCALE);
    image.convertTo(image, CV_64F);
    ptr = &image;

    out = contrastEnhancementFilter(ptr);
    imwrite("OutputImages/contrastEnhancement.jpg", out);

    //=============================================================

    return 0;
}
