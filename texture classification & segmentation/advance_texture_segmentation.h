/*
Version of OS - Ubuntu 18.04.3 LTS
IDE - Visual Studio Code for C++
Compiler- g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Linker - g++
Image Viewing Software - ImageJ (http://rsb.info.nih.gov/ij/)

Copyright © 2020 Yogesh Gajjar. All rights reserved.
*/



#ifndef ADVANCED_H
#define ADVANCED_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;
class AdvanceSegmentation {
private:
    unsigned char **Image;
    int height, width, window, padding, **lawFilters, filterCount, **finallawFilter, filterWindow, filterPadding;
    vector<string> fileName;
    vector<string> fileNameTest;
    double **intermediateImage, **featureVector, average, **newfeatureVector, **imageMeanSubtracted, **ImagewithBoundary, **finalfeatureVector, **zeromeanDataMatrix;

public:
    AdvanceSegmentation(int, int, int);
    void readImage(string, unsigned char *);
    void writeImage(string, unsigned char *);
    void generateImage(unsigned char *);
    unsigned char **generateMatrix2D(int, int);
    int **generateMatrix2DInt(int,int);
    double **generateMatrix2DDouble(int,int);
    double **boundaryExtension(double **, int);
    void delete2D(unsigned char **, int);
    void delete2DDouble(double **, int);
    void delete2DInt(int **, int);
    void subtractImageMean();
    void applyLawFilters();
    void calculateEnergy(int);
    unsigned char *kMeans(int);
    void PCA(string);
};

#endif