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
3.  Run and execute the code and then write "./output input_image.raw output_image.raw height width windowsize
4.  Use the Image Viewing Software to see the results.

*/ 

#include <opencv2/core/utility.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <algorithm> 
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <math.h>
#include "texture_segmentation.h"


using namespace std;
using namespace cv;




int main(int argc, char *argv[]) {
    int height{atoi(argv[3])}, width{atoi(argv[4])}, featureWidth{15}, windowSize{atoi(argv[5])};
    string file_input{argv[1]}, file_output7{argv[2]};

    string ext = "temp.raw";
    string filename;
    vector<string> fileName;
    for(int i = 1; i < 16; i++) {
        stringstream ss;
        ss << i;
        filename = ss.str() + ext;
        fileName.push_back(filename);
    }

    Segmentation *feature = new Segmentation(height, width, windowSize);
    unsigned char *Imagedata = new unsigned char[height*width];
    feature->readImage(file_input, Imagedata);
    feature->generateImage(Imagedata);
    feature->subtractImageMean();
    feature->applyLawFilters();
    feature->calculateEnergy(featureWidth, fileName);
    feature->writeImage(file_output7, feature->kMeans(14));

    delete feature;
    delete [] Imagedata;
    return 0;
}