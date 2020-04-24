/*.
Copyright © 2020 Yogesh Gajjar. All rights reserved.
*/



#include <iostream>
#include "error_diffusion.h"
#include <string>

using namespace std;


int main(int argc, char *argv[]) {
    int height{atoi(argv[5])}, width{atoi(argv[6])};
    string file_input{argv[1]}, fileOutputFS{argv[2]}, fileOutputJJN{argv[3]}, fileOutputS{argv[4]};
    ErrorDiffusion *error = new ErrorDiffusion{height, width, 0};
    unsigned char *ImageData = new unsigned char[height*width];
    error->readImage(file_input, ImageData);
    error->generateImage(ImageData);
    error->writeImage(fileOutputFS, error->errorFS());
    error->writeImage(fileOutputJJN, error->errorJJN());
    error->writeImage(fileOutputS, error->errorS());
    delete ImageData;
    delete error;
    return 0;
}