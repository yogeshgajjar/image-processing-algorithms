/*
Version of OS - Ubuntu 18.04.3 LTS
IDE - Visual Studio Code for C++
Compiler- g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Linker - g++
Image Viewing Software - ImageJ (http://rsb.info.nih.gov/ij/)

Copyright © 2020 Yogesh Gajjar. All rights reserved.
*/


#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Classification {
private:
    unsigned char **Image, **ImagewithBoundary, **fixedThresh;
    int height, width, window, padding, **lawFilters, filterCount, trainCount, testCount;
    vector<string> fileName;
    vector<string> fileNameTest;
    double **intermediateImage, **featureVector, minFilter, maxFilter, average, **newfeatureVector, **zeromeanDataMatrix, **imageMeanSubtracted;

public:
    Classification(int, int, int);
    void readImage(char *, unsigned char *);
    void writeImage(string, unsigned char **);
    void generateImage(unsigned char *);
    unsigned char **generateMatrix2D(int, int);
    int **generateMatrix2DInt(int,int);
    double **generateMatrix2DDouble(int,int);
    unsigned char **boundaryExtension(double **);
    void delete2D(unsigned char **, int);
    void delete2DDouble(double **, int);
    void delete2DInt(int **, int);
    void subtractImageMean();
    void applyLawFilters();
    void calculateEnergy(int, int);
    void generateFilename(int);
    void convert25Dto15D(int);
    void computePCA(int, string);
    void generateFilenameTest();
    void kMeans15D(int);
    void kMeans3D(int);
    void deleteAll();
};

#endif
