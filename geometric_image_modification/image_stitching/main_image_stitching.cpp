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
5.  Image dimensions are 720x480x3 where  720 is height, 480 is width, 3 is byteperpixel 
6.  Output image dimension are 1200 x 2000 x 3 where 1200 is height, 2000 is width & 3 is byteperpixel 
*/ 

#include <iostream>
#include <vector>
#include <string>
#include "image_stitching.h"

using namespace std;


int main(int argc, char *argv[]) {

    int height{atoi(argv[5])}, width{atoi(argv[6])}, BytesPerPixel{atoi(argv[7])};
    string file_il{argv[1]}, file_im{argv[2]}, file_ir{argv[3]}, file_oh_1{argv[4]};  
    Stitching *stitch = new Stitching{height, width, BytesPerPixel};
   
    unsigned char *ImageDataL = new unsigned char[height*width*BytesPerPixel];
    unsigned char *ImageDataM = new unsigned char[height*width*BytesPerPixel];
    unsigned char *ImageDataR = new unsigned char[height*width*BytesPerPixel];
    stitch->readImage(file_il, ImageDataL);
    stitch->readImage(file_ir, ImageDataR);
    stitch->readImage(file_im, ImageDataM);
    stitch->generateImageL(ImageDataL);
    stitch->generateImageM(ImageDataM);
    stitch->generateImageR(ImageDataR);
    stitch->writeImage(file_oh_1, stitch->homographicTransformation());
    stitch->delete3D(stitch->homographicTransformation(), 1200, 2000);

    delete ImageDataL;
    delete stitch;
    return 0;
}