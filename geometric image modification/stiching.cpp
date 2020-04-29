/*
Version of OS - Ubuntu 18.04.3 LTS
IDE - Visual Studio Code for C++
Compiler- g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Linker - g++
Image Viewing Software - ImageJ (http://rsb.info.nih.gov/ij/)

Copyright © 2020 Yogesh Gajjar. All rights reserved.


STEPS TO RUN THE CODE 
1.  Open terminal and change the directory where the source code files are downloaded. 
2.  Compile the codes using: "g++ sourcecode.cpp -o output 
3.  Run and execute the code and then write "./output left_image.raw middle_image.raw right_image.raw final_output.raw height width ByteperPixels
4.  Use the Image Viewing Software to see the results.

*/ 

#include <iostream>
#include <vector>
#include <string>
#include "image_stiching.h"

using namespace std;


int main(int argc, char *argv[]) {

    int height{atoi(argv[5])}, width{atoi(argv[6])}, BytesPerPixel{atoi(argv[7])};
    string file_il{argv[1]}, file_im{argv[2]}, file_ir{argv[3]}, file_oh_1{argv[4]};  
    Stiching *stich = new Stiching{height, width, BytesPerPixel};
   
    unsigned char *ImageDataL = new unsigned char[height*width*BytesPerPixel];
    unsigned char *ImageDataM = new unsigned char[height*width*BytesPerPixel];
    unsigned char *ImageDataR = new unsigned char[height*width*BytesPerPixel];
    stich->readImage(file_il, ImageDataL);
    stich->readImage(file_ir, ImageDataR);
    stich->readImage(file_im, ImageDataM);
    stich->generateImageL(ImageDataL);
    stich->generateImageM(ImageDataM);
    stich->generateImageR(ImageDataR);
    stich->writeImage(file_oh_1, stich->homographicTransformation());
    stich->delete3D(stich->homographicTransformation(), 1200, 2000);

    delete ImageDataL;
    delete stich;
    return 0;
}