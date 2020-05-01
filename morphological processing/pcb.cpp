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
3.  Run and execute the code and then write "./output input_image.raw output_image.raw height width 
4.  Use the Image Viewing Software to see the results.

*/

#include <iostream>
#include <vector>
#include <cmath> 
#include <algorithm> 
#include <string>
#include <unordered_map> 
#include <map> 
#include "pcb_analysis.h"

using namespace std;

int main(int argc, char **argv) {
    int height{atoi(argv[3])}, width{atoi(argv[4])};
    string file_i{argv[1]}, file_o{argv[2]}; 
    PCB *pcb = new PCB{height, width, 3};
   
    unsigned char *ImageData = new unsigned char[height*width];
 
    pcb->readImage(file_i, ImageData);
    pcb->generateImage(ImageData);
    pcb->fixedThresholding();
    pcb->finalCount();
    pcb->writeImage(file_o, pcb->imageRetrival());
    pcb->delete2D(pcb->imageRetrival(), height);

    delete ImageData;
    delete pcb;
    return 0;
}


