/*
Version of OS - Ubuntu 18.04.3 LTS
IDE - Visual Studio Code for C++
Compiler- g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Linker - g++
Image Viewing Software - ImageJ (http://rsb.info.nih.gov/ij/)

Copyright © 2020 Yogesh Gajjar. All rights reserved.

*/


#ifndef SHRINKING_H
#define SHRINKING_H

#include <iostream>
#include <vector>
#include <cmath> 
#include <algorithm> 
#include <string>

using namespace std;

class STK {
private:
    unsigned char **Image, **fixedThresh, **ImagewithBoundary, **finalOutput, **M;
    int height, width, window, padding, iterations, *starCount, *condTemp, *uncondTemp, count{}, temp{}, preCount{};

public:
    STK(int, int, int);
    void readImage(string, unsigned char *);
    void writeImage(string, unsigned char **);
    void generateImage(unsigned char *);
    unsigned char **generateMatrix2D(int, int);
    unsigned char **boundaryExtension();
    void delete2D(unsigned char **, int);
    void fixedThresholding();
    int conditionalCheck(char, int *);
    int unconditionalCheck(string, int *);
    void finalCount();
    void stage1ImageRetrival();
    unsigned char **stage2ImageRetrival();
};

#endif