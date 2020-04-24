/*
Version of OS - Ubuntu 18.04.3 LTS
IDE - Visual Studio Code for C++
Compiler- g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Linker - g++
Image Viewing Software - ImageJ (http://rsb.info.nih.gov/ij/)

Copyright © 2020 Yogesh Gajjar. All rights reserved.

*/


#ifndef _DITHERING_
#define _DITHERING_

#include <string>

using namespace std;

class Dithering {
private:
    unsigned char **Image, **fixedThresh, **randomThresh, **matrixI2, **matrixI8, **matrixI32, **matrixIndexing, **ditheringImage;
    int height, width, indexSize;

public:
    Dithering(int, int, int);
    void readImage(string, unsigned char *);
    void writeImage(string, unsigned char **);
    void generateImage(unsigned char *);
    unsigned char **generateMatrix(int, int);
    unsigned char **fixedThresholding();
    unsigned char **randomThresholding();
    void generateIndexMatrix(int);
    unsigned char **ditheringMatrix(int);
    void delete2D(unsigned char **, int);
};


#endif
