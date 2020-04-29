/*
Version of OS - Ubuntu 18.04.3 LTS
IDE - Visual Studio Code for C++
Compiler- g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Linker - g++
Image Viewing Software - ImageJ (http://rsb.info.nih.gov/ij/)

Copyright © 2020 Yogesh Gajjar. All rights reserved.

*/

#ifndef WARPING_H
#define WARPING_H


#include <string>
#include <iostream> 

using namespace std;

class Warping {
private:
    unsigned char ***Image, ***imageWarp, ***imageorig;
    int height, width, BytesPerPixel;
    double x,y,u,v, *arrayU, *arrayV;

public:
    Warping(int, int, int);
    void readImage(string, unsigned char *);
    void writeImage(string, unsigned char ***);
    void generateImage(unsigned char *);
    unsigned char ***generateMatrix3D(int, int, int);
    unsigned char ***diskWarping();
    unsigned char ***planeWarping();
    int signum(double);
    void delete3D(unsigned char ***, int, int);
};


#endif