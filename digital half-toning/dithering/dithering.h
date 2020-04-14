#include <string>

#ifndef _DITHERING_
#define _DITHERING_

using namespace std;

class Dithering {
private:
    unsigned char **Image, **fixedThresh, **randomThresh, **matrixI2, **matrixI8, **matrixI32, **matrixIndexing, **ditheringImage;
    int height, width, indexSize;

public:
    Dithering(int, int, int);
    void readImage(string, unsigned char *);
    void writeImage(string, unsigned char **);
    void generateImage(unsigned char *);
    unsigned char **generateMatrix(int, int);
    unsigned char **fixedThresholding();
    unsigned char **randomThresholding();
    void generateIndexMatrix(int);
    unsigned char **ditheringMatrix(int);
};


#endif
