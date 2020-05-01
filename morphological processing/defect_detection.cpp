/*
Version of OS - Ubuntu 18.04.3 LTS
IDE - Visual Studio Code for C++
Compiler- g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Linker - g++
Image Viewing Software - ImageJ (http://rsb.info.nih.gov/ij/)

Copyright © 2020 Yogesh Gajjar. All rights reserved.

*/
#include "defect_detection.h"
#include <iostream>
#include <vector>
#include <cmath> 
#include <algorithm> 
#include <string>
#include <unordered_map> 
#include <unordered_set>
#include <math.h>
using namespace std;

 DefectDetection:: DefectDetection(int h, int w, int win) {
    height = h;
    width = w;
    window = win;
    padding = win/2;
    iterations = 6; 
    iterations1 = 35;
    condTemp = new int[window*window];
    uncondTemp = new int[window*window];
    starCount = new int[iterations];
    countHoles = new int[window*window];
    midpointX = 0;
    midpointY = 0;
}

void  DefectDetection::readImage(string fileName, unsigned char *image) {
    FILE *file;
    if (!(file=fopen(fileName.data(),"rb"))) {
		cout << "Cannot open file: " << fileName.data() <<endl; 
		exit(1);
	}

	fread(image, sizeof(unsigned char), height*width, file);
	fclose(file);
}

void  DefectDetection::writeImage(string filename, unsigned char **final) {
    FILE *file;

    unsigned char finalImg[height*width];
    long int count{};
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            finalImg[count] = final[i][j];
            count++;
        }
    }

    if (!(file=fopen(filename.data(),"wb"))) {
		cout << "Cannot open file: " << filename.data() << endl;
		exit(1);
	}
	fwrite(finalImg, sizeof(unsigned char), height*width, file);
	fclose(file);
}

unsigned char ** DefectDetection::generateMatrix2D(int h, int w) {
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

void  DefectDetection::generateImage(unsigned char *main_image) {
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

void  DefectDetection::delete2D(unsigned char **image, int h) {
	for (int i = 0; i < h; i++) delete[] image[i];
    delete[] image;
}

void DefectDetection::fixedThresholding() {
    fixedThresh = generateMatrix2D(height, width);
    fixedThreshLines = generateMatrix2D(height, width);
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            if(Image[i][j] < 128) {
                fixedThresh[i][j] = 0;
                fixedThreshLines[i][j] = 1;
            }
            else {
                fixedThresh[i][j] = 1;
                fixedThreshLines[i][j] = 0;
            }
        }
    }
}

unsigned char **DefectDetection::boundaryExtension() {
    ImagewithBoundary = generateMatrix2D((2*padding)+height, (2*padding)+width);

    long int count{};
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            ImagewithBoundary[i+padding][j+padding] = Image[i][j];
            count++;
        }
    }
    return ImagewithBoundary;
}

int DefectDetection::conditionalCheck(char filter, int *C) {
    int flag{};
    string strTemp{};
    string maskS[66] ={"001010000","100010000","000010100","000010001", //S
                         "000011000","010010000","000110000","000010010", //S
                         "001011000","011010000","110010000","100110000", "000110100","000010110","000010011","000011001", //S
                         "110011000","010011001","011110000","001011010", //ST
                         "011011000","110110000","000110110","000011011", //ST
                         "110011001","011110100", //ST

                         "011011011","111111000","110110110","000111111", //STK
                         "111011011","011011111","111111100","111111001", "111110110","110110111","100111111","001111111", //STK
                         "111011111","111111101","111110111","101111111", //STK
                         "001011001","111010000","100110100","000010111", //STK
                         "111011000","011011001","111110000","110110100", "100110110","000110111","000011111","001011011", //STK
                         "111011001","111110100","100110111","001011111", //STK

                         "010011000","010110000","000110010","000011010", //TK
                         "111111011","111111110","110111111","011111111"};// K  
   
    
    if(filter == 'S') {
        for(int i=0; i < 9; i++) {
            strTemp += to_string(C[i]);
            if (strTemp.size() == 9) {
                // cout << strTemp << endl;
                for(int j=0; j < 58; j++) {
                    if(strTemp == maskS[j]) flag++;
                }
            }
        }
    }

    else if(filter == 'T'){
        for(int i=0; i < 9; i++) {
            strTemp += to_string(C[i]);
            for(int j=16; j < 62; j++) {
                if(strTemp == maskS[j]) flag++;
            }
        }
    }
    else {
        for(int i=0; i < 9; i++) {
            strTemp += to_string(C[i]);
            for(int j=26; j < 66; j++) {
                if(strTemp == maskS[j]) flag++;
            }
        }
    }

    return flag;       
}

int DefectDetection::unconditionalCheck(string filter, int *U) {
    int flag{};
    if (filter == "ST"){
        //for spur pattern
        if(U[0]==0 && U[1]==0 && U[2]==1 && U[3]==0 && U[4]==1 && U[5]==0 && U[6]==0 && U[7]==0 && U[8]==0) flag++;
        if(U[0]==1 && U[1]==0 && U[2]==0 && U[3]==0 && U[4]==1 && U[5]==0 && U[6]==0 && U[7]==0 && U[8]==0) flag++;

        //For single 4-connection pattern
        if(U[0]==0 && U[1]==0 && U[2]==0 && U[3]==0 && U[4]==1 && U[5]==0 && U[6]==0 && U[7]==1 && U[8]==0) flag++;
        if(U[0]==0 && U[1]==0 && U[2]==0 && U[3]==0 && U[4]==1 && U[5]==1 && U[6]==0 && U[7]==0 && U[8]==0) flag++;

        //For L-cluster pattern
        if(U[0]==0 && U[1]==0 && U[2]==1 && U[3]==0 && U[4]==1 && U[5]==1 && U[6]==0 && U[7]==0 && U[8]==0) flag++;
        if(U[0]==0 && U[1]==1 && U[2]==1 && U[3]==0 && U[4]==1 && U[5]==0 && U[6]==0 && U[7]==0 && U[8]==0) flag++;
        if(U[0]==1 && U[1]==1 && U[2]==0 && U[3]==0 && U[4]==1 && U[5]==0 && U[6]==0 && U[7]==0 && U[8]==0) flag++;
        if(U[0]==1 && U[1]==0 && U[2]==0 && U[3]==1 && U[4]==1 && U[5]==0 && U[6]==0 && U[7]==0 && U[8]==0) flag++;
        if(U[0]==0 && U[1]==0 && U[2]==0 && U[3]==1 && U[4]==1 && U[5]==0 && U[6]==1 && U[7]==0 && U[8]==0) flag++;
        if(U[0]==0 && U[1]==0 && U[2]==0 && U[3]==0 && U[4]==1 && U[5]==0 && U[6]==1 && U[7]==1 && U[8]==0) flag++;
        if(U[0]==0 && U[1]==0 && U[2]==0 && U[3]==0 && U[4]==1 && U[5]==0 && U[6]==0 && U[7]==1 && U[8]==1) flag++;
        if(U[0]==0 && U[1]==0 && U[2]==0 && U[3]==0 && U[4]==1 && U[5]==1 && U[6]==0 && U[7]==0 && U[8]==1) flag++;

        //For 4-connected offset pattern
        if(U[0]==0 && U[1]==1 && U[2]==1 && U[3]==1 && U[4]==1 && U[5]==0 && U[6]==0 && U[7]==0 && U[8]==0) flag++;
        if(U[0]==1 && U[1]==1 && U[2]==0 && U[3]==0 && U[4]==1 && U[5]==1 && U[6]==0 && U[7]==0 && U[8]==0) flag++;
        if(U[0]==0 && U[1]==1 && U[2]==0 && U[3]==0 && U[4]==1 && U[5]==1 && U[6]==0 && U[7]==0 && U[8]==1) flag++;
        if(U[0]==0 && U[1]==0 && U[2]==1 && U[3]==0 && U[4]==1 && U[5]==1 && U[6]==0 && U[7]==1 && U[8]==0) flag++;

        //For Spur corner cluster pattern
        if(U[0]==0 && (U[1]==1||U[5]==1) && U[2]==1 && U[3]==0 && U[4]==1 && U[6]==1 && U[7]==0 && U[8]==0) flag++;
        if(U[0]==1 && (U[1]==1||U[3]==1) && U[2]==0 && U[4]==1 && U[5]==0 && U[6]==0 && U[7]==0 && U[8]==1) flag++;
        if(U[0]==0 && U[1]==0 && U[2]==1 && (U[3]==1||U[7]==1) && U[4]==1 && U[5]==0 && U[6]==1 && U[8]==0) flag++;
        if(U[0]==1 && U[1]==0 && U[2]==0 && U[3]==0 && U[4]==1 && (U[5]==1||U[7]==1) && U[6]==0 && U[8]==1) flag++;

        //For Corner cluster pattern
        if(U[0]==1 && U[1]==1 && (U[2]==0 || U[2]==1) && U[3]==1 && U[4]==1 && (U[5]==1 || U[5]==0) && (U[6]==1 || U[6]==0) && (U[7]==1 || U[7]==0) && (U[8]==1 || U[8]==0)) flag++;


        //For Tee Branch pattern
        if(U[1]==1 && U[2]==0 && U[3]==1 && U[4]==1 && U[5]==1 && U[7]==0 && U[8]==0) flag++;
        if(U[0]==0 && U[1]==1 && U[3]==1 && U[4]==1 && U[5]==1 && U[6]==0 && U[7]==0) flag++;
        if(U[0]==0 && U[1]==0 && U[3]==1 && U[4]==1 && U[5]==1 && U[6]==0 && U[7]==1) flag++;
        if(U[1]==0 && U[2]==0 && U[3]==1 && U[4]==1 && U[5]==1 && U[7]==1 && U[8]==0) flag++;
        if(U[1]==1 && U[3]==1 && U[4]==1 && U[5]==0 && U[6]==0 && U[7]==1 && U[8]==0) flag++;
        if(U[0]==0 && U[1]==1 && U[2]==0 && U[3]==1 && U[4]==1 && U[5]==0 && U[7]==1) flag++;
        if(U[0]==0 && U[1]==1 && U[2]==0 && U[3]==0 && U[4]==1 && U[5]==1 && U[7]==1) flag++;
        if(U[1]==1 && U[3]==0 && U[4]==1 && U[5]==1 && U[6]==0 && U[7]==1 && U[8]==0) flag++;

        //For Vee Branch pattern
        if(U[0]==1 && U[2]==1 && U[4]==1 && (U[6]==1|| U[7]==1|| U[8]==1)) flag++;
        if(U[0]==1 && (U[2]==1|| U[5]==1|| U[8]==1) && U[4]==1 && U[6]==1) flag++;
        if((U[0]==1|| U[1]==1|| U[2]==1) && U[4]==1 && U[6]==1 && U[8]==1) flag++;
        if((U[0]==1|| U[3]==1|| U[6]==1) && U[2]==1 && U[4]==1 && U[8]==1) flag++;

        //For Diagonal Branch pattern
        if((U[0]==1 || U[0]==0) && U[1]==1 && U[2]==0 && U[3]==0 && U[4]==1 && U[5]==1 && U[6]==1 && U[7]==0 && (U[8]==1 || U[8]==0)) flag++;
        if(U[0]==0 && U[1]==1 && (U[2]==1 || U[2]==0) && U[3]==1 && U[4]==1 && U[5]==0 && (U[6]==1 || U[6]==0) && U[7]==0 && U[8]==1) flag++;
        if((U[0]==1 || U[0]==0) && U[1]==0 && U[2]==1 && U[3]==1 && U[4]==1 && U[5]==0 && U[6]==0 && U[7]==1 && (U[8]==1 || U[8]==0)) flag++;
        if(U[0]==1 && U[1]==0 && (U[2]==1 || U[2]==0) && U[3]==0 && U[4]==1 && U[5]==1 && (U[6]==1 || U[6]==0) && U[7]==1 && U[8]==0) flag++;

    }     

    if(filter == "K") {
        //For spur pattern
        if(U[0]==0 && U[1]==0 && U[2]==0 && U[3]==0 && U[4]==1 && U[5]==0 && U[6]==0 && U[7]==0 && U[8]==1) flag++;
        if(U[0]==0 && U[1]==0 && U[2]==0 && U[3]==0 && U[4]==1 && U[5]==0 && U[6]==1 && U[7]==0 && U[8]==0) flag++;
        if(U[0]==0 && U[1]==0 && U[2]==1 && U[3]==0 && U[4]==1 && U[5]==0 && U[6]==0 && U[7]==0 && U[8]==0) flag++;
        if(U[0]==1 && U[1]==0 && U[2]==0 && U[3]==0 && U[4]==1 && U[5]==0 && U[6]==0 && U[7]==0 && U[8]==0) flag++;

        //For single 4 connection pattern
        if(U[0]==0 && U[1]==0 && U[2]==0 && U[3]==0 && U[4]==1 && U[5]==0 && U[6]==0 && U[7]==1 && U[8]==0) flag++;
        if(U[0]==0 && U[1]==0 && U[2]==0 && U[3]==0 && U[4]==1 && U[5]==1 && U[6]==0 && U[7]==0 && U[8]==0) flag++;
        if(U[0]==0 && U[1]==0 && U[2]==0 && U[3]==1 && U[4]==1 && U[5]==0 && U[6]==0 && U[7]==0 && U[8]==0) flag++;
        if(U[0]==0 && U[1]==1 && U[2]==0 && U[3]==0 && U[4]==1 && U[5]==0 && U[6]==0 && U[7]==0 && U[8]==0) flag++;

        // For L corner pattern
        if(U[0]==0 && U[1]==1 && U[2]==0 && U[3]==0 && U[4]==1 && U[5]==1 && U[6]==0 && U[7]==0 && U[8]==0) flag++;
        if(U[0]==0 && U[1]==1 && U[2]==0 && U[3]==1 && U[4]==1 && U[5]==0 && U[6]==0 && U[7]==0 && U[8]==0) flag++;
        if(U[0]==0 && U[1]==0 && U[2]==0 && U[3]==0 && U[4]==1 && U[5]==1 && U[6]==0 && U[7]==1 && U[8]==0) flag++;
        if(U[0]==0 && U[1]==0 && U[2]==0 && U[3]==1 && U[4]==1 && U[5]==0 && U[6]==0 && U[7]==0 && U[8]==0) flag++;

        //For corner cluster pattern 
        if(U[0]==1 && U[1]==1 && (U[2]==0 || U[2]==1) && U[3]==1 && U[4]==1 && (U[5]==1 || U[5]==0) && (U[6]==1 || U[6]==0) && (U[7]==1 || U[7]==0) && (U[8]==1 || U[8]==0)) flag++;
        if((U[0]==1 || U[0]==0) && (U[1]==1 || U[1]==0) && (U[2]==1 || U[2]==0) && (U[3]==1 || U[3]==0) && U[4]==1 && U[5]==1 && (U[6]==0 || U[6]==1) && U[7]==1 && U[8]==1) flag++;

        //For tee branch pattern
        if(U[1]==1 && U[3]==1 && U[4]==1 && U[5]==1) flag++;
        if(U[1]==1 && U[3]==1 && U[4]==1 && U[7]==1) flag++;
        if(U[3]==1 && U[4]==1 && U[5]==1 && U[7]==1) flag++;
        if(U[1]==1 && U[4]==1 && U[5]==1 && U[7]==1) flag++;

        //For vee branch pattern 
        if(U[0]==1 && (U[6]==1 || U[7]==1 || U[8]==1) && U[2]==1 && U[4]==1) flag++;
        if(U[0]==1 && (U[2]==1 || U[5]==1 || U[8]==1) && U[4]==1 && U[6]==1) flag++;
        if((U[0]==1 || U[1]==1 || U[2]==1) && U[4]==1 && U[6]==1 && U[8]==1) flag++;
        if((U[0]==1 || U[3]==1 || U[6]==1) && U[2]==1 && U[4]==1 && U[8]==1) flag++;

        //For diagonal branch pattern
        if((U[0]==1 || U[0]==0) && U[1]==1 && U[2]==0 && U[3]==0 && U[4]==1 && U[5]==1 && U[6]==1 && U[7]==0 && (U[8]==1 || U[8]==0)) flag++;
        if(U[0]==0 && U[1]==1 && (U[2]==1 || U[2]==0) && U[3]==1 && U[4]==1 && U[5]==0 && (U[6]==1 || U[6]==0) && U[7]==0 && U[8]==1) flag++;
        if((U[0]==1 || U[0]==0) && U[1]==0 && U[2]==1 && U[3]==1 && U[4]==1 && U[5]==0 && U[6]==0 && U[7]==1 && (U[8]==1 || U[8]==0)) flag++;
        if(U[0]==1 && U[1]==0 && (U[2]==1 || U[2]==0) && U[3]==0 && U[4]==1 && U[5]==1 && (U[6]==1 || U[6]==0) && U[7]==1 && U[8]==0) flag++;
    }
    return flag;
}

void DefectDetection::finalCount(unsigned char **image, string filter1, char filter2, int iter, int flag) {
    int count{};
    vector<int> coordinatesX; 
    vector<int> coordinatesY;
    int trial{}, val{}, preVal{};
    string str;
    
    M = generateMatrix2D(height, width);
    finalOutput = generateMatrix2D(height, width);

    for(int p = 1; p < iter; p++) {
        for(int i=0; i < height; i++) {
            for(int j=0; j < width; j++) {
                if(image[i][j] == 1) {
                    int index{0};
                    for(int l= i-1;l <= i+1; l++) {
                        for(int m= j-1; m <= j+1; m++) {
                            int r{l}, c{m};
                            if(r < 0) r = 0;
                            if(r > height-1) r = height-1;
                            if(c < 0) c = 0;
                            if(c > width-1) c = width-1;
                            condTemp[index] = (int)image[r][c];
                            index++;
                        }
                    }
                    int count = conditionalCheck(filter2, condTemp);
                    if(count == 1) {
                        M[i][j] = 1;
                        trial++;
                    }
                    else M[i][j] = 0;
                }
                else M[i][j] = 0;
            }
        }
        for(int i=0; i < height; i++) {
            for(int j=0; j < width; j++) {
                if(M[i][j] == 0) finalOutput[i][j] = image[i][j];
                else {
                    int index{0};
                    for(int l = i-1; l <= i+1; l++) {
                        for(int m = j-1; m <= j+1; m++) {
                            int r{l}, c{m};
                            if(r < 0) r = 0;
                            if(r > height-1) r = height-1;
                            if(c < 0) c = 0;
                            if(c > width-1) c = width-1;
                            uncondTemp[index] = (int)M[r][c];
                            index++;
                        }
                    }
                    int count = unconditionalCheck(filter1, uncondTemp);
                    if(count == 1) finalOutput[i][j] = 1;
                    else finalOutput[i][j] = 0;
                    image[i][j] = finalOutput[i][j];
                }
            }
        }
        // 
        count = 0;
        for(int i=0; i < height; i++) {
            for(int j=0; j < width; j++) {
                if(finalOutput[i][j] == 1 && i > 60 && j > 60 && i < 170 && j < 170) {
                    int index{0};
                    for(int l = i-1; l <= i+1; l++) {
                        for(int m = j-1; m <= j+1; m++) {
                            int r{l}, c{m};
                            if(r < 0) r = 0;
                            if(r > height-1) r = height-1;
                            if(c < 0) c = 0;
                            if(c > width-1) c = width-1;
                            countHoles[index] = (int)finalOutput[r][c];
                            index++;
                        }
                    }
                    str = "\0";
                    for(int x=0; x < 9; x++) {
                        str += to_string(countHoles[x]);
                    }
                    if(str == "000010000") {
                        count++;
                        
                        if(heightIndex.find(i) == heightIndex.end()) {
                            heightIndex.insert(i);
                            coordinatesX.push_back(i);
                        
                            // cout << i ;
                        }
                        if(widthIndex.find(j) == widthIndex.end()) {
                            widthIndex.insert(j);
                            coordinatesY.push_back(j);
                        }
                    }
                }
            }
        }
    }
    
    if (flag == 1){
        coordinatesX.erase(coordinatesX.begin()+0);
        coordinatesY.erase(coordinatesY.begin()+0);
        cout << "\n" << endl;
        for(int i=0; i < coordinatesX.size(); i++) {
            cout << "The coordinates of the center location of four black holes: " << "[" << coordinatesX[i] << "," << coordinatesY[i] << "]" << endl;
        }
        
        midpointX = (coordinatesX.front() + coordinatesX.back())/2;
        midpointY = (coordinatesY.front() + coordinatesY.back())/2;
        cout << "Hole count : " << count-1 << endl;
        cout << "\n";
    }
    if(flag == 0) {
        cout << "------------------------------\n" << endl;
        midPoint();
    }
    
    
    delete condTemp;
    delete uncondTemp;
    delete countHoles;
    delete starCount;
}

unsigned char **DefectDetection::imageRetrival() {
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            finalOutput[i][j] = finalOutput[i][j]*255;
        }
    }
    return finalOutput;
}

void DefectDetection::calculateRadius() {
    finalCount(fixedThreshLines, "ST", 'S', iterations1, 1);
    finalCount(fixedThresh, "ST", 'T', iterations, 0);

    
}

void DefectDetection::midPoint() {
    string str;
    int coordinateX, coordinateY;
    int *countHoles = new int[window*window];
    int count = 0;
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            if(finalOutput[i][j] == 1) {
                coordinateX = i;
                coordinateY = j;
                break;
            }
        }
    }
    int radius = sqrt((midpointX-coordinateX)*(midpointX-coordinateX) + (midpointY-coordinateY)*(midpointY-coordinateY));
    cout << "Radius of the gear tooth is : " << radius << endl;
    int next_coordinateX{}, next_coordinateY{}; 
    cout << "Midpoint coordinates are : " << "[" << midpointX << "," << midpointY << "]" << endl;
    cout << "--------------------------------" << endl;
    vector<int> angles = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 360};
    for(auto i:angles) {
        auto temp = (i*M_PI)/180.0;
        next_coordinateX = midpointX + (radius * cos(temp));
        next_coordinateY = midpointY + (radius * sin(temp));
        if(finalOutput[next_coordinateX][next_coordinateY] != 1) {
            cout << "The coordinates of missing teeth are :" << "[" << next_coordinateX << "," << next_coordinateY << "]" << " at angle: " << i << endl;
        }
    }
    delete countHoles;
}