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
3.  This code works for all morphological process. Only the filter name in the unconditional and conditional function changes. 
    For conditional mask checking, char filter 'S' is for Shrinking, 'T' is for Thinning & 'K' is for Skeletonizing. 
    For unconditional mask checking, string filter "ST" is for Shrinking and Thinning and "K" is for Skeletonizing. 
4.  To run this code, you must change the argument at line 337 and 373
    for "Shrinking", select conditionalCheck('S', condTemp) and unconditionalCheck("ST", uncondTemp)
    for "Thinning", select conditionalCheck('T', condTemp) and unconditionalCheck("ST", uncondTemp)
    for "Skeletonizing", select conditionalCheck('K', condTemp) and conditionalCheck("K", uncondTemp)
3.  Run and execute the code and then write "./output input_image.raw stage2_output.raw height width 
4.  Use the Image Viewing Software to see the results.

*/

#include "shrinking.h"
#include <iostream>
#include <string>

using namespace std;


int main(int argc, char **argv) {
    int height{atoi(argv[3])}, width{atoi(argv[4])};
    string file_i{argv[1]}, file_stage2{argv[2]}; 
    STK *stk = new STK{height, width, 3};
   
    unsigned char *ImageData = new unsigned char[height*width];
    stk->readImage(file_i, ImageData);
    stk->generateImage(ImageData);
    stk->fixedThresholding();
    stk->finalCount();
    stk->writeImage(file_stage2, stk->stage2ImageRetrival());
    // stk->writeImage(file_stage1, stk->stage1ImageRetrival());

    stk->delete2D(stk->stage2ImageRetrival(), height);
    // stk->delete2D(stk->stage1ImageRetrival(), height);

    delete[] ImageData;
    delete[] stk;
    return 0;
}


