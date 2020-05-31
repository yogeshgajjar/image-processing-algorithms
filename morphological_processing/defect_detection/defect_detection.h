/*
Version of OS - Ubuntu 18.04.3 LTS
IDE - Visual Studio Code for C++
Compiler- g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Linker - g++
Image Viewing Software - ImageJ (http://rsb.info.nih.gov/ij/)

Copyright © 2020 Yogesh Gajjar. All rights reserved.

*/


#ifndef DEFECT_H
#define DEFECT_H

#include <string>
#include <unordered_set>

using namespace std;

class DefectDetection {
private:
    unsigned char **Image, **fixedThresh, **ImagewithBoundary, **finalOutput, **M, **image_labels, **fixedThreshLines;
    int height, width, window, padding, iterations, iterations1, *condTemp, *uncondTemp, *starCount, *countHoles, midpointX, midpointY;
    unordered_set<int> heightIndex;
    unordered_set<int> widthIndex;

public:
    DefectDetection(int, int, int);
    void readImage(string, unsigned char *);
    void writeImage(string, unsigned char **);
    void generateImage(unsigned char *);
    unsigned char **generateMatrix2D(int, int);
    unsigned char **boundaryExtension();
    void delete2D(unsigned char **, int);
    void fixedThresholding();
    int conditionalCheck(char, int *);
    int unconditionalCheck(string, int *);
    void finalCount(unsigned char **, string, char, int, int);
    unsigned char **imageRetrival();
    void calculateRadius();
    void midPoint();
};

#endif