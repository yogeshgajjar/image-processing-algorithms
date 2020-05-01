/*
Version of OS - Ubuntu 18.04.3 LTS
IDE - Visual Studio Code for C++
Compiler- g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Linker - g++
Image Viewing Software - ImageJ (http://rsb.info.nih.gov/ij/)

Copyright © 2020 Yogesh Gajjar. All rights reserved.
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

Segmentation::Segmentation(int h, int w, int win) {
    height = h;
    width = w;
    window = win;
    padding = win/2;
    filterCount = 25;
    average = 0;
    filterWindow = 5;
    filterPadding = filterWindow/2;
}

void Segmentation::readImage(string fileName, unsigned char *image) {
    FILE *file;
    if (!(file=fopen(fileName.data(),"rb"))) {
		cout << "Cannot open file: " << fileName.data() <<endl; 
		exit(1);
	}

	fread(image, sizeof(unsigned char), height*width, file);
	fclose(file);	
    // cout << "File: " << fileName << " has been read successfully." << endl;
}

void Segmentation::writeImage(string filename, unsigned char *final) {
    FILE *file;

    // unsigned char finalImg[height*width];
    // long int count{};
    // for(int i=0; i < height; i++) {
    //     for(int j=0; j < width; j++) {
    //         finalImg[count] = final[i][j];
    //         count++;
    //     }
    // }

    if (!(file=fopen(filename.data(),"wb"))) {
		cout << "Cannot open file: " << filename.data() << endl;
		exit(1);
	}
	fwrite(final, sizeof(unsigned char), height*width, file);
	fclose(file);
}

unsigned char **Segmentation::generateMatrix2D(int h, int w) {
    long int count{};
    unsigned char **image = new unsigned char *[h];
    for(int i=0; i < h; i++) {
        image[i] = new unsigned char [w];
        for(int j=0; j < w; j++) {
            image[i][j] = 0;
            count++;
        }
    }
    return image;
}

int **Segmentation::generateMatrix2DInt(int h, int w) {
    long int count{};
    int **image = new int *[h];
    for(int i=0; i < h; i++) {
        image[i] = new int [w];
        for(int j=0; j < w; j++) {
            image[i][j] = 0;
            count++;
        }
    }
    return image;
}

double **Segmentation::generateMatrix2DDouble(int h, int w) {
    long int count{};
    double **image = new double *[h];
    for(int i=0; i < h; i++) {
        image[i] = new double [w];
        for(int j=0; j < w; j++) {
            image[i][j] = 0;
            count++;
        }
    }
    return image;
}


void Segmentation::generateImage(unsigned char *main_image) {
    long int count{};
    Image = new unsigned char *[height];
    for(int i = 0; i < height; i++) {
        Image[i] = new unsigned char [width];
        for(int j = 0; j < width; j++) {
            Image[i][j] = main_image[count];
            count++;
        }
    }
}

double **Segmentation::boundaryExtension(double **image, int flag) {

    if (flag == 1) {
        ImagewithBoundary = generateMatrix2DDouble((2*filterPadding)+height, (2*filterPadding)+width);
        long int count{};
        //center
        for(int i=0; i < height; i++) {
            for(int j=0; j < width; j++) {
                ImagewithBoundary[i+filterPadding][j+filterPadding] = image[i][j];
                count++;
            }
        }
        for(int i=0; i < filterPadding; i++) {
            for(int j=0; j < width; j++) {
                ImagewithBoundary[i][j+filterPadding] = image[filterPadding -i-1][j];
                ImagewithBoundary[height+(2*filterPadding)-i-1][j+filterPadding] = image[height-filterPadding+i][j];
            }
        }

        for(int i=0; i < filterPadding; i++) {
            for(int j=0; j < height+(2*filterPadding); j++) {
                ImagewithBoundary[j][i] = ImagewithBoundary[j][(2*filterPadding)-i];
                ImagewithBoundary[j][width+(2*filterPadding)-i-1] = ImagewithBoundary[j][width+(2*filterPadding)-1-(2*filterPadding)+i];
            }
        }
    }
    else {
        ImagewithBoundary = generateMatrix2DDouble((2*padding)+height, (2*padding)+width);
        long int count{};
        //center
        for(int i=0; i < height; i++) {
            for(int j=0; j < width; j++) {
                ImagewithBoundary[i+padding][j+padding] = image[i][j];
                count++;
            }
        }
        for(int i=0; i < padding; i++) {
            for(int j=0; j < width; j++) {
                ImagewithBoundary[i][j+padding] = image[padding -i-1][j];
                ImagewithBoundary[height+(2*padding)-i-1][j+padding] = image[height-padding+i][j];
            }
        }

        for(int i=0; i < padding; i++) {
            for(int j=0; j < height+(2*padding); j++) {
                ImagewithBoundary[j][i] = ImagewithBoundary[j][(2*padding)-i];  //2*padding - i
                ImagewithBoundary[j][width+(2*padding)-i-1] = ImagewithBoundary[j][width+(2*padding)-1-(2*padding)+i];
            }
        }
    }
    return ImagewithBoundary;
}



void Segmentation::delete2D(unsigned char **image, int h) {
	for (int i = 0; i < h; i++) delete[] image[i];
    delete[] image;
}

void Segmentation::delete2DDouble(double **image, int h) {
	for (int i = 0; i < h; i++) delete[] image[i];
    delete[] image;
}

void Segmentation::delete2DInt(int **image, int h) {
	for (int i = 0; i < h; i++) delete[] image[i];
    delete[] image;
}


void Segmentation::subtractImageMean() {
    imageMeanSubtracted = generateMatrix2DDouble(height, width);
    int meanSum{};
    float meanVal{};

    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            meanSum = 0;
            for(int l = i-padding; l <= i+padding; l++) {
                for(int m= j-padding; m <= j+padding; m++) {
                    int a = l;
                    int b = m;
                    if(a < 0) a = 0;
                    if(a > height-1) a = height-1;
                    if(b < 0) b = 0;
                    if(b > width-1) b = width - 1;
                    meanSum += Image[a][b];
                }
            }
            meanVal = meanSum / (window*window);
            imageMeanSubtracted[i][j] = abs(Image[i][j] - meanVal);
        }
    }
}


void Segmentation::applyLawFilters() {
    int filterInitialization[5][5] = {{1,4,6,4,1}, 
                                        {-1,-2,0,2,1},
                                        {-1,0,2,0,-1},
                                        {-1,2,0,-2,1}, 
                                        {1,-4,6,-4,1}};
    int row = sizeof(filterInitialization)/sizeof(filterInitialization[0]);
    lawFilters = generateMatrix2DInt(25,25);

    for(int i=0; i < row; i++) {
        for(int j=0; j < row; j++) {
            for(int k=0; k < row; k++) {
                for(int l=0; l < row; l++) {
                    lawFilters[i*row+j][k*row+l] = filterInitialization[i][k] * filterInitialization[j][l];
                }
            }
        }
    }
    finallawFilter = generateMatrix2DInt(15,25);
    for(int j=0; j < 25; j++) {
        finallawFilter[0][j] = lawFilters[0][j];
        finallawFilter[1][j] = lawFilters[1][j]; 
        finallawFilter[2][j] = lawFilters[7][j];
        finallawFilter[3][j] = lawFilters[6][j];
        finallawFilter[4][j] = lawFilters[2][j];
        finallawFilter[5][j] = lawFilters[8][j];
        finallawFilter[6][j] = lawFilters[12][j];
        finallawFilter[7][j] = lawFilters[3][j];
        finallawFilter[8][j] = lawFilters[9][j];
        finallawFilter[9][j] = lawFilters[18][j];
        finallawFilter[10][j] = lawFilters[4][j];
        finallawFilter[11][j] = lawFilters[13][j];
        finallawFilter[12][j] = lawFilters[24][j];
        finallawFilter[13][j] = lawFilters[19][j];
        finallawFilter[14][j] = lawFilters[14][j];
    }
}

void Segmentation::calculateEnergy(int count, vector<string> file) {
    featureVector = generateMatrix2DDouble(height*width, count);  //change the variables
    newfeatureVector = generateMatrix2DDouble(height*width, count);
    finalfeatureVector = generateMatrix2DDouble(height*width, 14);
    double **temp, **temp1;
    temp = generateMatrix2DDouble((2*filterPadding)+height, (2*filterPadding)+width);
    temp1 = generateMatrix2DDouble((2*padding)+height, (2*padding)+width);
    intermediateImage = generateMatrix2DDouble(height, width);

    unsigned char **image = NULL;
    image = generateMatrix2D(height, width);
    
    temp = boundaryExtension(imageMeanSubtracted, 1);  // can be potential case for segmentation fault

    for(int y=0; y < count; y++) {
        for(int i=0; i < height; i++) {
            for(int j=0; j < width; j++) {
                double sum = 0;
                for(int l=0; l < filterWindow; l++) {
                    for(int m=0; m < filterWindow; m++) {
                        sum += temp[i+l][j+m] * finallawFilter[y][l*5+m]; 
                    }
                }
                intermediateImage[i][j] = sum;
            }
        }

        temp1 = boundaryExtension(intermediateImage, 0);
        
        for(int i=0; i < height; i++) {
            for(int j=0; j < width; j++) {
                average = 0;
                for(int l=0; l < window; l++) {
                    for(int m=0; m < window; m++) {
                        average += (temp1[i+l][j+m] * temp1[i+l][j+m]);
                    }
                }
                featureVector[i*width+j][y] = average / (window*window);
            }
        }
    }

    
    // finalfeatureVector = featureVector;
    for(int i=0; i < width*height; i++) {
        for(int j=0; j < count; j++) {
            newfeatureVector[i][j] = (featureVector[i][j] / (featureVector[i][0]));
        }
    }

    for(int i=0; i < width*height; i++) {
        for(int j=0; j < 14; j++) {
            finalfeatureVector[i][j] = newfeatureVector[i][j+1];
        }
    }    

    delete2DDouble(temp, height+(2*filterPadding));
    delete2DDouble(temp1, height+(2*padding));
}

unsigned char *Segmentation::kMeans(int count) {

    int *imageLabel = new int[height*width];
    double **centroid = NULL;
    double **newCentroid = NULL;
    int labelCount{6};
    centroid = generateMatrix2DDouble(labelCount, count);
    newCentroid = generateMatrix2DDouble(labelCount, count);
    double minValue{};
    

    for(int i=0; i < count; i++) {
        centroid[0][i] = finalfeatureVector[6700][i];    
        centroid[1][i] = finalfeatureVector[30690][i];  
        centroid[2][i] = finalfeatureVector[32976][i];  
        centroid[3][i] = finalfeatureVector[67500][i];  
        centroid[4][i] = finalfeatureVector[29880][i];  
        centroid[5][i] = finalfeatureVector[174573][i]; 
    }
    // int temp1, temp2, temp3, temp4, temp5, temp6;
    for(int itr=0; itr< 20; itr++) {
        int label[labelCount] = {0};
        double temp1=0,temp2=0,temp3=0,temp4=0,temp5=0,temp6=0;
        for(int i=0; i < height*width; i++) {
            double distance[labelCount] = {0};
            
            for(int p=0; p < labelCount; p++) {
                for(int j=0; j < count; j++) {
                    distance[p] += sqrt(pow((centroid[p][j] - finalfeatureVector[i][j]),2));
                }
            }
            minValue = distance[0];
            for(int x=0; x < labelCount; x++) {
                if(distance[x] < minValue) {
                    minValue = distance[x];
                }
            }

            if(minValue == distance[0]) {
                imageLabel[i] = 0;
                for(int t=0; t < count; t++) newCentroid[0][t] += finalfeatureVector[i][t];
                label[0]++;
                temp1++;
            }
            if(minValue == distance[1]) {
                imageLabel[i] = 1;
                for(int t=0; t < count; t++) newCentroid[1][t] += finalfeatureVector[i][t];
                label[1]++;
                temp2++;
            }
            if(minValue == distance[2]) {
                imageLabel[i] = 2;
                for(int t=0; t < count; t++) newCentroid[2][t] += finalfeatureVector[i][t];
                label[2]++;
                temp3++;
            }
            if(minValue == distance[3]) {
                imageLabel[i] = 3;
                for(int t=0; t < count; t++) newCentroid[3][t] += finalfeatureVector[i][t];
                label[3]++;
                temp4++;
            }
            if(minValue == distance[4]) {
                imageLabel[i] = 4;
                for(int t=0; t < count; t++) newCentroid[4][t] += finalfeatureVector[i][t];
                label[4]++;
                temp5++;
            }
            if(minValue == distance[5]) {
                imageLabel[i] = 5;
                for(int t=0; t < count; t++) newCentroid[5][t] += finalfeatureVector[i][t];
                label[5]++;
                temp6++;
            }
        }

        for(int a=0; a < labelCount; a++) {
            for(int b=0; b < count; b++) {
                centroid[a][b] = newCentroid[a][b] / label[a];
                newCentroid[a][b] = 0;
            }
        }
    }
    

    int class1{}, class2{}, class3{}, class4{}, class5{}, class6{};
    for(int i=0; i < height*width; i++) {
        if(imageLabel[i] == 0) class1++;
        if(imageLabel[i] == 1) class2++;
        if(imageLabel[i] == 2) class3++;
        if(imageLabel[i] == 3) class4++;
        if(imageLabel[i] == 4) class5++;
        if(imageLabel[i] == 5) class6++;
    }

    cout << class1 << " " << class2 << " " << class3 << " " << class4 << " " << class5 << " " << class6 << endl;
    vector<int> outputGray = {0, 51, 102, 153, 204, 255};
    unsigned char *output = new unsigned char[height*width];

    for(int i=0; i < height*width; i++) {
        output[i] = outputGray[imageLabel[i]];
    }


    delete [] imageLabel;
    delete2D(Image, height);
    delete2DDouble(intermediateImage, height);
    delete2DDouble(centroid, labelCount);
    delete2DDouble(newCentroid, labelCount);
    delete2DDouble(featureVector, height*width);
    delete2DDouble(newfeatureVector, height*width);
    delete2DDouble(finalfeatureVector, height*width);
    delete2DDouble(imageMeanSubtracted, height);
    delete2DInt(lawFilters, 25);
    delete2DInt(finallawFilter, 15);
    return output;
}
