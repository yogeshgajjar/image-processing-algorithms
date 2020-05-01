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
3.  The next step contains flag input. If selected 0 uses the morphological method and if selected 1 uses connected component labelling method. 
3.  Run and execute the code and then write "./output inout_image.raw output_image.raw height width flag
4.  Use the Image Viewing Software to see the results.

*/

#include "counting_stars.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char **argv) {
    int height{atoi(argv[3])}, width{atoi(argv[4])}, flag{atoi(argv[5])};
    string file_i{argv[1]}, file_o{argv[2]}; 
    CountingStars *count = new CountingStars{height, width, 3};
   
    unsigned char *ImageData = new unsigned char[height*width];
 
    count->readImage(file_i, ImageData);
    count->generateImage(ImageData);
    count->fixedThresholding();

    if (flag == 1) {
        cout << "---------Connected Component Labelling -----------\n" << endl;
        count->stage1CCL();
        count->stage2CCL();
        count->stage2CCL();
        count->cclResult();
    }
    else {
        cout << "------Morphological Method for Star Counting------ \n" << endl;
        count->finalCount();
        count->writeImage(file_o, count->imageRetrival());
        count->histogramPrint();
        count->delete2D(count->imageRetrival(), height);
    }   

    delete ImageData;
    delete count;
    return 0;
}
