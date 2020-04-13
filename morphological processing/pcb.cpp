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
    Problem 2c. PCB Analysis
------------------------------------------------------------------------
STEPS TO RUN THE CODE 
1.  Open terminal and change the directory where the source code files are downloaded. 
2.  Compile the codes using: "g++ sourcecode.cpp -o output 
3.  Run and execute the code and then write "./output input_image.raw output_image.raw height width 
4.  Use the Image Viewing Software to see the results.

*/

#include <iostream>
#include <vector>
#include <cmath> 
#include <algorithm> 
#include <string>
#include <unordered_map> 
#include <map> 

using namespace std;

class PCB {
private:
    unsigned char **Image, **fixedThresh, **ImagewithBoundary, **finalOutput, **M;
    int height, width, window, padding, iterations;

public:
    PCB(int, int, int);
    void readImage(string, unsigned char *);
    void writeImage(string, unsigned char **);
    void generateImage(unsigned char *);
    unsigned char **generateMatrix2D(int, int);
    unsigned char **boundaryExtension();
    void delete2D(unsigned char **, int);
    void fixedThresholding();
    int conditionalCheck(char, int *);
    int unconditionalCheck(string, int *);
    void finalCount();
    unsigned char **imageRetrival();
};

PCB::PCB(int h, int w, int win) {
    height = h;
    width = w;
    window = win;
    padding = win/2;
    iterations = 4;  //4
}

void PCB::readImage(string fileName, unsigned char *image) {
    FILE *file;
    if (!(file=fopen(fileName.data(),"rb"))) {
		cout << "Cannot open file: " << fileName.data() <<endl; 
		exit(1);
	}

	fread(image, sizeof(unsigned char), height*width, file);
	fclose(file);
}

void PCB::writeImage(string filename, unsigned char **final) {
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

unsigned char **PCB::generateMatrix2D(int h, int w) {
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

void PCB::generateImage(unsigned char *main_image) {
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

void PCB::delete2D(unsigned char **image, int h) {
	for (int i = 0; i < h; i++) delete[] image[i];
    delete[] image;
}

void PCB::fixedThresholding() {
    fixedThresh = generateMatrix2D(height, width);
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            if(Image[i][j] < 128) fixedThresh[i][j] = 0;
            else fixedThresh[i][j] = 1;
        }
    }
}

unsigned char **PCB::boundaryExtension() {
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

int PCB::conditionalCheck(char filter, int *C) {
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

int PCB::unconditionalCheck(string filter, int *U) {
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

void PCB::finalCount() {
    int count{};
    int *condTemp = new int[window*window];
    int *uncondTemp = new int[window*window];
    int *starCount = new int[iterations];
    int *countHoles = new int[window*window];
    int trial{}, val{}, preVal{};
    string str;
    M = generateMatrix2D(height, width);
    finalOutput = generateMatrix2D(height, width);

    for(int p = 1; p < iterations; p++) {
        for(int i=0; i < height; i++) {
            for(int j=0; j < width; j++) {
                if(fixedThresh[i][j] == 1) {
                    int index{0};
                    for(int l= i-1;l <= i+1; l++) {
                        for(int m= j-1; m <= j+1; m++) {
                            int r{l}, c{m};
                            if(r < 0) r = 0;
                            if(r > height-1) r = height-1;
                            if(c < 0) c = 0;
                            if(c > width-1) c = width-1;
                            condTemp[index] = (int)fixedThresh[r][c];
                            index++;
                        }
                    }
                    int count = conditionalCheck('S', condTemp);
                    if(count == 1) {
                        M[i][j] = 1;
                        trial++;
                    }
                    else M[i][j] = 0;
                }
                else M[i][j] = 0;
            }
        }
        // cout << trial << endl;
        for(int i=0; i < height; i++) {
            for(int j=0; j < width; j++) {
                if(M[i][j] == 0) finalOutput[i][j] = fixedThresh[i][j];
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
                    int count = unconditionalCheck("ST", uncondTemp);
                    if(count == 1) finalOutput[i][j] = 1;
                    else finalOutput[i][j] = 0;
                    fixedThresh[i][j] = finalOutput[i][j];
                }
            }
        }
        count = 0;
        for(int i=0; i < height; i++) {
            for(int j=0; j < width; j++) {
                if(finalOutput[i][j] == 1) {
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
                    if(str == "000010000") count++;
                }
            }
        }
    }
    cout << "Hole count : " << count << endl;
    
    delete condTemp;
    delete uncondTemp;
    delete countHoles;
    delete starCount;
}

unsigned char **PCB::imageRetrival() {
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            finalOutput[i][j] = finalOutput[i][j]*255;
        }
    }
    return finalOutput;
}




int main(int argc, char **argv) {
    int height{atoi(argv[3])}, width{atoi(argv[4])};
    string file_i{argv[1]}, file_o{argv[2]}; 
    PCB *pcb = new PCB{height, width, 3};
   
    unsigned char *ImageData = new unsigned char[height*width];
 
    pcb->readImage(file_i, ImageData);
    pcb->generateImage(ImageData);
    pcb->fixedThresholding();
    pcb->finalCount();
    pcb->writeImage(file_o, pcb->imageRetrival());
    pcb->delete2D(pcb->imageRetrival(), height);

    delete ImageData;
    delete pcb;
    return 0;
}


