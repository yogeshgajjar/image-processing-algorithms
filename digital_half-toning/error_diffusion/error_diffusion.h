/*.
Copyright © 2020 Yogesh Gajjar. All rights reserved.
*/


#include <string>
#ifndef _ERROR_
#define _ERROR_

class ErrorDiffusion {
private:
    unsigned char **Image, **imageTemp, **imageTempBinarized, **ImagewithBoundary;
    int height, width, window, padding;
    long double error;

public:
    ErrorDiffusion(int, int, long double);
    unsigned char **boundaryExtension(int);
    void readImage(std::string, unsigned char *);
    void writeImage(std::string, unsigned char **);
    void generateImage(unsigned char *);
    unsigned char **generateMatrix(int, int);
    unsigned char **errorFS();
    unsigned char **errorJJN();
    unsigned char **errorS();
    ~ErrorDiffusion();  
};

#endif