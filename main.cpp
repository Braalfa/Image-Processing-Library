#include "Base/baseFilters.hpp"

int main(int argc, char** argv)
{
    Mat image = imread("InputImages/barbara.jpg",
                        IMREAD_COLOR);
    Mat* ptr = &image;
    Mat out = medianFilter(ptr);
    imwrite("OutputImages/medianFilter.jpg", out);

    image.convertTo(image, CV_64F);
    out =averageFilter(ptr, 30); 
    imwrite("OutputImages/averageFilter.jpg", out);

    image = imread("InputImages/imagen11.jpg",
                    IMREAD_GRAYSCALE);
    image.convertTo(image, CV_64F);
    ptr = &image;

    out = contrastEnhancementFilter(ptr);
    imwrite("OutputImages/contrastEnhancement.jpg", out);

    image = imread("InputImages/Columnas.jpg",
                   IMREAD_GRAYSCALE);
    image.convertTo(image, CV_64F);
    ptr = &image;

    out = bordersEnhancementFilter(ptr);   
    imwrite("OutputImages/borderEnhancement.jpg", out);
    return 0;
}