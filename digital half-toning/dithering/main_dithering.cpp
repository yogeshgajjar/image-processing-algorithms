/*
Version of OS - Ubuntu 18.04.3 LTS
IDE - Visual Studio Code for C++
Compiler- g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Linker - g++
Image Viewing Software - ImageJ (http://rsb.info.nih.gov/ij/)

Copyright © 2020 Yogesh Gajjar. All rights reserved.

*/


#include <iostream>
#include "dithering.h"


int main(int argc, char *argv[]) {
    int height{atoi(argv[5])}, width{atoi(argv[6])}, index{atoi(argv[7])};
    string file_input{argv[1]}, fileOutFixed{argv[2]}, fileOutRandom{argv[3]},fileOutDithering{argv[4]};
    Dithering *dithering = new Dithering{height, width, 0};
    unsigned char *Imagedata = new unsigned char[height*width];
    dithering->readImage(file_input, Imagedata);
   
    dithering->generateImage(Imagedata);
    dithering->writeImage(fileOutFixed, dithering->fixedThresholding());
    dithering->writeImage(fileOutRandom, dithering->randomThresholding()); 
    dithering->writeImage(fileOutDithering, dithering->ditheringMatrix(index));

    delete dithering;
    delete Imagedata;
    return 0;
}