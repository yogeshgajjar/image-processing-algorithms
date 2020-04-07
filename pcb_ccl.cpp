/*
#EE569 Homework Assignment #3
#Date: 20th February 2020
#Name: Yogesh Sanat Gajjar
#USCID: 5476153636
#Email: ygajjar@usc.edu

Version of OS - Ubuntu 18.04.3 LTS
IDE - Visual Studio Code for C++
Compiler- g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Linker - g++
Image Viewing Software - ImageJ (http://rsb.info.nih.gov/ij/)

------------------------------------------------------------------------
    Problem 2c. PCB Analysis (CCL way) 
------------------------------------------------------------------------
STEPS TO RUN THE CODE 
1.  Open terminal and change the directory where the source code files are downloaded. 
2.  Compile the codes using: "g++ sourcecode.cpp -o output 
3.  Run and execute the code and then write "./output input_image.raw output_image.raw height width 
4.  Use the Image Viewing Software to see the results.

*/

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <math.h>
#include <vector>


using namespace std;

class CCL {
private:
    unsigned char **Image, **fixedThresh, **ImagewithBoundary, **data, **map;
    int height, width, padding, window, section, new_height, new_width;
    vector<int> unique;

public:
    CCL(int, int, int);
    void readImage(string, unsigned char *);
    void writeImage(string, unsigned char **);
    void generateImage(unsigned char *);
    unsigned char **generateMatrix2D(int, int);
    void removeBoundary();
    void delete2D(unsigned char **, int);
    void fixedThresholding();
    void findLabels();
    void repass();
    void printOutput();
    unsigned char **imageRetrival();
    void mergee(int, int, int, unsigned char **, int, int);
    void ccl();
};

CCL::CCL(int h, int w, int win) {
    height = h;
    width = w;
    window = win;
    padding = win/2;
    section = 0;
    new_height = height - (2*padding);
    new_width = width - padding - 1;
}

void CCL::readImage(string fileName, unsigned char *image) {
    FILE *file;
    if (!(file=fopen(fileName.data(),"rb"))) {
		cout << "Cannot open file: " << fileName.data() <<endl; 
		exit(1);
	}

	fread(image, sizeof(unsigned char), height*width, file);
	fclose(file);
}

void CCL::writeImage(string filename, unsigned char **final) {
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
	fwrite(finalImg, sizeof(unsigned char), new_height*new_width, file);
	fclose(file);
}

unsigned char **CCL::generateMatrix2D(int h, int w) {
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

void CCL::generateImage(unsigned char *main_image) {
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

void CCL::delete2D(unsigned char **image, int h) {
	for (int i = 0; i < h; i++) delete[] image[i];
    delete[] image;
}

void CCL::fixedThresholding() {
    fixedThresh = generateMatrix2D(height, width);
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            if(Image[i][j] < 127) fixedThresh[i][j] = 1;
            else fixedThresh[i][j] = 0;
        }
    }
}

void CCL::removeBoundary() {
    ImagewithBoundary = generateMatrix2D(height-(2*padding), (width-padding-1));

    long int count{};
    for(int i=padding; i < height-padding; i++) {
        for(int j=padding; j < width-padding; j++) {
            ImagewithBoundary[i-padding][j-padding] = fixedThresh[i][j];
            count++;
        }
    }
}

void CCL::findLabels() {
    map = generateMatrix2D(height, width);
    data = generateMatrix2D(height-(2*padding), (width-padding-1));
    data = ImagewithBoundary;
    // int i{}, j{};
    for(int i=0; i < height-(2*padding); i++) {
        for(int j=0; j < width-(padding-1); j++) {
            if(data[i][j] == 1) {
                int old_section = section + 1;
                if ((i - 1 >= 0) && (j - 1 >= 0) && data[i - 1][j - 1] == 1) {
                    int section = map[(i-1)][(j-1)];
                    if(section < old_section)
                        old_section = section;
                }
                if ((i - 1 >= 0) && data[i - 1][j] == 1) {
                    int section = map[(i-1)][(j)];
                    if(section < old_section)
                        old_section = section;
                }
                if (i - 1 >= 0 && j + 1 < (width-padding-1) && data[i - 1][j + 1] == 1) {
                    int section = map[(i-1)][(j+1)];
                    if(section < old_section)
                        old_section = section;
                }
                if (j - 1 >= 0 && data[i][j - 1] == 1) {
                    int section = map[(i)][(j-1)];
                    if(section < old_section)
                        old_section = section;
                }
                if(old_section > section)
                    section = old_section;
                map[i][j] = old_section;
            }
        }
    }

}

void CCL::repass() {
    unique.clear();
    for(int i=0; i< height-(2*padding);i++){
        for(int j=0; j<width-padding-1;j++){
            if(data[i][j] != 1)
                continue;
            int min = map[i][j];
            // cout << min << endl;
            if(i - 1 >= 0 && j - 1 >= 0 && map[(i-1)][(j-1)] != 0){
                int section =map[(i-1)][(j-1)];
                if(section < min)
                    min = section;
            }
            if(j - 1 >= 0 && map[(i)][(j-1)] != 0){
                int section = map[(i)][(j-1)];
                if(section < min)
                    min = section;
            }
            if(i + 1 < (height-2*padding) && j - 1 >= 0 && map[(i+1)][(j-1)] != 0){
                int section = map[(i+1)][(j-1)];
                if(section < min)
                    min = section;
            }
            if(i - 1 >= 0 && map[(i-1)][(j)] != 0){
                int section = map[(i-1)][(j)];
                if(section < min)
                    min = section;
            }
            if(i + 1 < height-(2*padding) && map[(i+1)][(j)] != 0){
                int section = map[(i+1)][(j)];
                if(section < min)
                    min = section;
            }
            if(i - 1 >= 0 && j + 1 < width && map[(i-1)][(j+1)] != 0){
                int section = map[(i-1)][(j+1)];
                if(section < min)
                    min = section;
            }
            if(j + 1 < (width-padding-1) && map[(i)][(j+1)] != 0){
                int section = map[(i)][(j+1)];
                if(section < min)
                    min = section;
            }
            if(i + 1 < height-(2*padding) && j + 1 < width && map[(i+1)][(j+1)] != 0){
                int section = map[(i+1)][(j+1)];
                if(section < min)
                    min = section;
            }
            map[i][j] = min;
            if(map[(i-1)][(j-1)] != 0)
                map[(i-1)][(j-1)] = min;
            if(map[(i)][(j-1)] != 0)
                map[(i)][(j-1)] = min;
            if(map[(i+1)][(j-1)] != 0)
                map[(i+1)][(j-1)] = min;
            if(map[(i-1)][(j)] != 0)
                map[(i-1)][(j)] = min;
            if(map[(i+1)][(j)] != 0)
                map[(i+1)][(j)] = min;
            if(map[(i-1)][(j+1)] != 0)
                map[(i-1)][(j+1)] = min;
            if(map[(i)][(j+1)] != 0)
                map[(i)][(j+1)] = min;
            if(map[(i+1)][(j+1)] != 0)
                map[(i+1)][(j+1)] = min;

            if (std::find(unique.begin(), unique.end(), min) == unique.end()) {
                unique.push_back(min);
            }
        }
    }
}

void CCL::printOutput() {
    cout << " " << endl;
    cout << " " << endl;
    cout << "The number of pathways in the PCB is: " << (unique.size()-11) << endl;
}

unsigned char **CCL::imageRetrival() {
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            fixedThresh[i][j] = fixedThresh[i][j]*255;
        }
    }
    return fixedThresh;
}



int main(int argc, char **argv) {
    int height{atoi(argv[3])}, width{atoi(argv[4])};
    string file_i{argv[1]}, file_o{argv[2]}; 
    CCL *ccl = new CCL{height, width, 5};
   
    unsigned char *ImageData = new unsigned char[height*width];
 
    ccl->readImage(file_i, ImageData);
    ccl->generateImage(ImageData);
    ccl->fixedThresholding();
    ccl->removeBoundary();
    // ccl->ccl();
    ccl->findLabels();
    for(int i=0; i < 100; i++) {
        ccl->repass();
        
    }
    ccl->printOutput();

    delete ImageData;
    delete ccl;
    return 0;
}
