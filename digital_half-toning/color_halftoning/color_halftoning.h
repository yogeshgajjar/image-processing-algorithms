
#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class ErrorDiffusion {
private:
    unsigned char ***Image, **imageTemp, **imageTempBinarized, **ImagewithBoundary, **imageCyan, **imageMagenta, **imageYellow, **R, **G, **B;
    int height, width, window, padding, BytesPerPixel;
    long double error;

public:
    ErrorDiffusion(int, int, long double, int, int);
    unsigned char **boundaryExtension(int, unsigned char **);
    void readImage(string, unsigned char *);
    void writeImage(string, unsigned char ***);
    void generateImage(unsigned char *);
    unsigned char **generateMatrix(int, int);
    unsigned char **errorFS(unsigned char **);
    void separateColor();
    void mergeRGB();
    unsigned char ***mergeColors();

    
};

#endif