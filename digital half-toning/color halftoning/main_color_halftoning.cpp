
#include "color_halftoning.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    int height{atoi(argv[3])}, width{atoi(argv[4])}, BytesPerPixel{atoi(argv[5])};
    string file_input{argv[1]}, fileOutputFS{argv[2]};
    ErrorDiffusion *error = new ErrorDiffusion{height, width, 0, BytesPerPixel, 3};
    unsigned char *ImageData = new unsigned char[height*width*BytesPerPixel];
    error->readImage(file_input, ImageData);
    error->generateImage(ImageData);
    error->separateColor();
    error->writeImage(fileOutputFS, error->mergeColors());
    delete ImageData;
    delete error;
    return 0;
}