/*
Version of OS - Ubuntu 18.04.3 LTS
IDE - Visual Studio Code for C++
Compiler- g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Linker - g++
Image Viewing Software - ImageJ (http://rsb.info.nih.gov/ij/)

Copyright © 2020 Yogesh Gajjar. All rights reserved.

ALGORITHM IMPLEMENTED
1.  Read the input image, height, width, BytePerPixel using command line 
2.  First, start with the conversion of square to disc. The height & width is reranged to [-1,1] and used the forward mapping formula to convert it to disc. As the formula used for forward 
mapping assumes the disc and square to be in the range [-1,1]. Stored the pixel values in a variable and printed it.  
3.  Next, performed the inverse mapping from disc to square image. First converted the square to disc(similar to step 2) and again converted it back to square to get the final image similar to the input
image. Used the inverse mapping formula after getting the pixel value of the disc. Stored the new pixel value in a variable and printed it.  
4.  Write the output onto .raw file.  


STEPS TO RUN THE CODE 
1.  Open terminal and change the directory where the source code files are downloaded. 
2.  Compile the codes using: "g++ sourcecode.cpp -o output 
3.  Run and execute the code and then write "./output input_image1.raw input_image2.raw input_image3.raw warped_output1.raw warped_output2.raw warped_output3.raw original_image_out1.raw original_image_out2.raw original_image_out3.rawheight width BytePerPixel
4.  Use the Image Viewing Software to see the results.

*/ 

#include "image_warping.h"
#include <iostream>
#include <vector>
#include <cmath> 
#include <algorithm> 
#include <string>

using namespace std;



int main(int argc, char *argv[]) {
    int height{atoi(argv[10])}, width{atoi(argv[11])}, BytesPerPixel{atoi(argv[12])};
    string file_ih{argv[1]}, file_ir{argv[2]}, file_ib{argv[3]}, file_oh_1{argv[4]}, file_or_1{argv[5]}, file_ob_1{argv[6]}, file_oh_2{argv[7]}, file_or_2{argv[8]}, file_ob_2{argv[9]}; 
    Warping *warp = new Warping{height, width, BytesPerPixel};
    unsigned char *ImageData = new unsigned char[height*width*BytesPerPixel];
    warp->readImage(file_ih, ImageData);
    warp->generateImage(ImageData);
    warp->writeImage(file_oh_1, warp->diskWarping());
    warp->writeImage(file_oh_2, warp->planeWarping());
    warp->delete3D(warp->diskWarping(), height, width);
    warp->delete3D(warp->planeWarping(), height, width);

    warp->readImage(file_ir, ImageData);
    warp->generateImage(ImageData);
    warp->writeImage(file_or_1, warp->diskWarping());
    warp->writeImage(file_or_2, warp->planeWarping());
    warp->delete3D(warp->diskWarping(), height, width);
    warp->delete3D(warp->planeWarping(), height, width);

    warp->readImage(file_ib, ImageData);
    warp->generateImage(ImageData);
    warp->writeImage(file_ob_1, warp->diskWarping());
    warp->writeImage(file_ob_2, warp->planeWarping());
    warp->delete3D(warp->diskWarping(), height, width);
    warp->delete3D(warp->planeWarping(), height, width);

    delete ImageData;
    delete warp;
    return 0;
}