/*
Version of OS - Ubuntu 18.04.3 LTS
IDE - Visual Studio Code for C++
Compiler- g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Linker - g++
Image Viewing Software - ImageJ (http://rsb.info.nih.gov/ij/)

This code belongs to Yogesh Gajjar
------------------------------------------------------------------------
                Problem 1b. Canny Edge Detector
------------------------------------------------------------------------
ALGORITHM IMPLEMENTED
1.  Read the input image using imread and initialize variables as Mat datatype
2.  Convert the image into grayscale image using the function CvtColor. 
3.  Call the canny function to generate edges. Note: Canny function has Guassian blur function inbuilt. Also, provide lower and upper threshold values. 
4.  Write the image into a file. 

STEPS TO RUN THE CODE 
1.  Open terminal and change the directory where the source code files are downloaded. 
2.  Compile the codes using: "g++ sourcecode.cpp -o output `pkg-config --cflags --libs opencv`"
3.  Modify the directory in at line no 72 to the current directory. 
3.  Run and execute the code and then write "./program low_threshold high_threshold" 
4.  Use the Image Viewing Software to see the results. 

*/ 


#include <iostream>
#include <stdio.h>
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

class CannyImage {
private:
    Mat gray, canny, image, thresh;
    int low, high;
public:
    CannyImage(int, int);
    void cannyImage();
    void readImage(string);
    void writeImage();
}; 

CannyImage::CannyImage(int l, int h) {
    low = l;
    high = h;
}

void CannyImage::readImage(string file) {
    image = imread(file.data());
}

void CannyImage::cannyImage() {
    cvtColor(image, gray, CV_RGB2GRAY);
    Canny(gray, canny, low, high);
    threshold(canny, thresh, 127, 255, THRESH_BINARY_INV);
}

void CannyImage::writeImage() {
    imshow("image", thresh);
    imwrite( "/home/yogesh/Git/hw2_edge_detection/final/canny.jpg", thresh);
    waitKey(0);
}


int main(int argc, char **argv) 
{
    if (argc != 4) 
    {
        cout << " Enter the image name: " << endl;
        return -1;
    }

    int low{atoi(argv[2])}, high{atoi(argv[3])};
    CannyImage canny{low, high};
    canny.readImage(argv[1]);
    canny.cannyImage();
    canny.writeImage();
    return 0;
}