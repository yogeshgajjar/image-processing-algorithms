/*
Version of OS - Ubuntu 18.04.3 LTS
IDE - Visual Studio Code for C++
Compiler- g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Linker - g++
Image Viewing Software - ImageJ (http://rsb.info.nih.gov/ij/)

Copyright © 2020 Yogesh Gajjar. All rights reserved.

*/


#ifndef STITCHING_H
#define STITCHING_H

#include <string> 

using namespace std;

class Stitching {
private:
    unsigned char ***Image, ***ImageL, ***ImageM, ***ImageR, ***imageOut;
    int height, width, BytesPerPixel, new_height{}, new_width{};

public:
    Stitching(int, int, int);
    void readImage(string, unsigned char *);
    void writeImage(string, unsigned char ***);
    void generateImageL(unsigned char *);
    void generateImageM(unsigned char *);
    void generateImageR(unsigned char *);
    unsigned char ***generateMatrix3D(int, int, int);
    void delete3D(unsigned char ***, int, int);
    unsigned char ***homographicTransformation();    

};


#endif