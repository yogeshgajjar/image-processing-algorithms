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
    Problem 2a. Basic morphological process implementation
------------------------------------------------------------------------
STEPS TO RUN THE CODE 
1.  Open terminal and change the directory where the source code files are downloaded. 
2.  Compile the codes using: "g++ sourcecode.cpp -o output 
3.  This code works for all morphological process. Only the filter name in the unconditional and conditional function changes. 
    For conditional mask checking, char filter 'S' is for Shrinking, 'T' is for Thinning & 'K' is for Skeletonizing. 
    For unconditional mask checking, string filter "ST" is for Shrinking and Thinning and "K" is for Skeletonizing. 
4.  To run this code, you must change the argument at line 337 and 373
    for "Shrinking", select conditionalCheck('S', condTemp) and unconditionalCheck("ST", uncondTemp)
    for "Thinning", select conditionalCheck('T', condTemp) and unconditionalCheck("ST", uncondTemp)
    for "Skeletonizing", select conditionalCheck('K', condTemp) and conditionalCheck("K", uncondTemp)
3.  Run and execute the code and then write "./output input_image.raw stage2_output.raw height width 
4.  Use the Image Viewing Software to see the results.

*/

#include <iostream>
#include <vector>
#include <cmath> 
#include <algorithm> 
#include <string>

using namespace std;

class STK {
private:
    unsigned char **Image, **fixedThresh, **ImagewithBoundary, **finalOutput, **M;
    int height, width, window, padding, iterations, *starCount, *condTemp, *uncondTemp, count{}, temp{}, preCount{};

public:
    STK(int, int, int);
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
    void stage1ImageRetrival();
    unsigned char **stage2ImageRetrival();
};

STK::STK(int h, int w, int win) {
    height = h;
    width = w;
    window = win;
    padding = win/2;
    iterations = 80;
    condTemp = new int[window*window];
    uncondTemp = new int[window*window];
    starCount = new int[iterations];

}

void STK::readImage(string fileName, unsigned char *image) {
    FILE *file;
    if (!(file=fopen(fileName.data(),"rb"))) {
		cout << "Cannot open file: " << fileName.data() <<endl; 
		exit(1);
	}

	fread(image, sizeof(unsigned char), height*width, file);
	fclose(file);
}

void STK::writeImage(string filename, unsigned char **final) {
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

unsigned char **STK::generateMatrix2D(int h, int w) {
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

void STK::generateImage(unsigned char *main_image) {
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

void STK::delete2D(unsigned char **image, int h) {
	for (int i = 0; i < h; i++) delete[] image[i];
    delete[] image;
}

void STK::fixedThresholding() {
    fixedThresh = generateMatrix2D(height, width);
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            if(Image[i][j] < 128) fixedThresh[i][j] = 0;
            else fixedThresh[i][j] = 1;
        }
    }
}

unsigned char **STK::boundaryExtension() {
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

int STK::conditionalCheck(char filter, int *C) {
    int flag{};
    string strTemp{};
    string maskS[66] ={"001010000","100010000","000010100","000010001", //S
                         "000011000","010010000","000110000","000010010", //S
                         "001011000","011010000","110010000","100110000", "000110100","000010110","000010011","000011001", //S
                         "110011000","010011001","011110000","001011010", //ST
                         "011011000","110110000","000110110","000011011", //ST
                         "110011001","011110100", //ST

                         "011011011","111111000","110110110","000111111", //STK

                         "111011011","011011111","111111100","111111001","111110110","110110111","100111111","001111111", //STK
                         "111011111","111111101","111110111","101111111", //STK
                         "001011001","111010000","100110100","000010111", //STK
                         "111011000","011011001","111110000","110110100","100110110","000110111","000011111","001011011", //STK
                         "111011001","111110100","100110111","001011111", //STK

                         "010011000","010110000","000110010","000011010", //TK
                         "111111011","111111110","110111111","011111111"};// K
    
    
    if(filter == 'S') {
        for(int i=0; i < 9; i++) {
            strTemp += to_string(C[i]);
            if (strTemp.size() == 9) {
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

int STK::unconditionalCheck(string filter, int *U) {
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

void STK::finalCount() {
    M = generateMatrix2D(height, width);
    finalOutput = generateMatrix2D(height, width);
    vector<int> intermediate;


    for(int i=0; i < iterations; i++) {
        intermediate.push_back(round(iterations/4));
    }

    
    for(int p=0; p < iterations; p++) {
        for(int i=0; i < height; i++) {
            for(int j=0; j < width; j++) {
                if(fixedThresh[i][j] == 1) {
                    for(int x=0; x < 9; x++) condTemp[x] = 0;
                    int index{0};
                    for(int l= i-1;l <= i+1; l++) {
                        for(int m= j-1; m <= j+1; m++) {
                            condTemp[index] = fixedThresh[l][m];
                            index++;
                        }
                    }
                    int count = conditionalCheck('K', condTemp);
                    if(count == 1) M[i][j] = 1;
                    else M[i][j] = 0;
                }
                else M[i][j] = 0;
            }
        }
        if( p == iterations/4 ) {
            stage1ImageRetrival(); 
            writeImage("stage1_1.raw", M);

        }
        if (p == iterations/2 ) {
            stage1ImageRetrival(); 
            writeImage("stage1_2.raw", M);
        }
        if (p == 3*iterations/4 ) {
            stage1ImageRetrival(); 
            writeImage("stage1_3.raw", M);
        }
        for(int i=0; i < height; i++) {
            for(int j=0; j < width; j++) {
                if(M[i][j] == 255) M[i][j] == 1;
            }
        }
        for(int i=0; i < height; i++) {
            for(int j=0; j < width; j++) {
                if(M[i][j] == 1) {
                    for(int x = 0; x < 9; x++) uncondTemp[x] = 0;
                    int index{0};
                    for(int l = i-1; l <= i+1; l++) {
                        for(int m = j-1; m <= j+1; m++) {
                            uncondTemp[index] = M[l][m];
                            index++;
                        }
                    }
                    int count = unconditionalCheck("K", uncondTemp);
                    if(count == 1) finalOutput[i][j] = 1;
                    else finalOutput[i][j] = 0;
                    fixedThresh[i][j] = finalOutput[i][j];
                }
                else finalOutput[i][j] = fixedThresh[i][j];
            }
        }
        if( p == iterations/4 ) {
            stage2ImageRetrival(); 
            writeImage("stage2_1.raw", finalOutput);

        }
        if (p == iterations/2 ) {
            stage2ImageRetrival(); 
            writeImage("stage2_2.raw", finalOutput);
        }
        if (p == 3*iterations/4 ) {
            stage2ImageRetrival(); 
            writeImage("stage2_3.raw", finalOutput);
        }
    }
    
    delete[] condTemp;
    delete[] uncondTemp;
}

unsigned char **STK::stage2ImageRetrival() {
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            finalOutput[i][j] = finalOutput[i][j]*255;
        }
    }
    return finalOutput;
}

void STK::stage1ImageRetrival() {

    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            M[i][j] = M[i][j] * 255;
        }
    }
    // return M;
}



int main(int argc, char **argv) {
    int height{atoi(argv[3])}, width{atoi(argv[4])};
    string file_i{argv[1]}, file_stage2{argv[2]}; 
    STK *stk = new STK{height, width, 3};
   
    unsigned char *ImageData = new unsigned char[height*width];
    stk->readImage(file_i, ImageData);
    stk->generateImage(ImageData);
    stk->fixedThresholding();
    stk->finalCount();
    stk->writeImage(file_stage2, stk->stage2ImageRetrival());
    // stk->writeImage(file_stage1, stk->stage1ImageRetrival());

    stk->delete2D(stk->stage2ImageRetrival(), height);
    // stk->delete2D(stk->stage1ImageRetrival(), height);

    delete[] ImageData;
    delete[] stk;
    return 0;
}


