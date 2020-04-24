
/*

Version of OS - Ubuntu 18.04.3 LTS
IDE - Visual Studio Code for C++
Compiler- g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Linker - g++
Image Viewing Software - ImageJ (http://rsb.info.nih.gov/ij/)

------------------------------------------------------------------------
                            Dithering 
------------------------------------------------------------------------
ALGORITHM IMPLEMENTED
1.  Read the inout image, height, width, BytePerPixel , index_value using command line 
2.  For fixed thresholding, consider a fixed threshold value eg. 127 and compare it with the input pixels. If found less than 127, map it to 0, else map it to 255. 
3.  For random thresholding, generate a random number using rand()%256. Compare the value of the random number in the range of 0 to 255 with the input image. If found greater than random value, 
map it to 255. Else map it to 0. 
4.  For dithering, generate Bayer matrix using the formalae provided in the lecture. Interate through the loop until count > 0. Recursively add elements in the Bayer filter matrix based on index provided. 
This will generate three matrix I2, I8, I32. 
5.  Compute the threshold matrix using the recursively generated bayer matrix and perform thresholding to generate an output file. 
5.  Write the individual output value onto a .raw file. 


STEPS TO RUN THE CODE 
1.  Open terminal and change the directory where the source code files are downloaded. 
2.  Compile the codes using: "g++ sourcecode.cpp -o output 
3.  Run and execute the code and then write "./output input_image.raw fixed_thresholding.raw random_thresholding.raw dithering_output.raw height width index(0 for I2, 1 for I8, 2 for I32)
4.  Use the Image Viewing Software to see the results.

*/ 

#include <iostream> 
#include <algorithm>
#include <cmath> 
#include <string> 
#include <vector>

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
    void generateMatrix(unsigned char **, int, int);
    unsigned char **fixedThresholding();
    unsigned char **randomThresholding();
    void generateIndexMatrix(int);
    unsigned char **ditheringMatrix(int);
    ~Dithering();

};

Dithering::Dithering(int h, int w, int i) {
    height = h;
    width = w;
    indexSize = i;
}

void Dithering::readImage(string fileName, unsigned char *image) {
    FILE *file;
    if (!(file=fopen(fileName.data(),"rb"))) {
		cout << "Cannot open file: " << fileName.data() <<endl; 
		exit(1);
	}

	fread(image, sizeof(unsigned char), height*width, file);
	fclose(file);
}

/* Method to write the image into image format */ 
void Dithering::writeImage(string filename, unsigned char **final) {
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

void Dithering::generateMatrix(unsigned char **image, int h, int w) {
    long int count{};
    image = new unsigned char *[h];
    for(int i=0; i < h; i++) {
        image[i] = new unsigned char [w];
        for(int j=0; j < w; j++) {
            image[i][j] = 0;
            count++;
        }
    }
    cout << "count: " << count << endl;
}

void Dithering::generateImage(unsigned char *main_image) {
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

unsigned char **Dithering::fixedThresholding() {
    generateMatrix(fixedThresh, height, width); 
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            if(Image[i][j] < 127) fixedThresh[i][j] = 0;
            else fixedThresh[i][j] = 255;
        }
    }
    return fixedThresh;
}

unsigned char **Dithering::randomThresholding() {
    generateMatrix(randomThresh, height, width);
    unsigned char random{};
    // randomThresh = Image;
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) { 
            random = rand()%256;
            if(Image[i][j] > random) randomThresh[i][j] = 255;
            else randomThresh[i][j] = 0;
        }
    }
    return randomThresh;
}

// void Dithering::instnatiateIndexMatrix() {}

void Dithering::generateIndexMatrix(int index) {
    // float matrixI2[2][2] = {{1,3}, {2,0}};
    unsigned char **matrix, **newMatrix;
    generateMatrix(matrixIndexing, index, index);
    int count{(log(index) / log(2))}, num{1}; 
    // cout << "Initial count: " << count;
    generateMatrix(matrix, num, num);  
    while(count > 0) {
        num *= 2;
        newMatrix = matrix;
        generateMatrix(matrix, num, num);
        for(int i=0; i < num; i++) {
            for(int j=0; j < num; j++) {
                if(i < num/2 && j < num/2) matrix[i][j] = 4 * newMatrix[i%(num/2)][j%(num/2)] + 1;
                else if(i < num/2 && j >= num/2) matrix[i][j] = 4 * newMatrix[i%(num/2)][j%(num/2)] + 2;
                else if(i >= num/2 && j < num/2) matrix[i][j] = 4 * newMatrix[i%(num/2)][j%(num/2)] + 3;
                else  matrix[i][j] = 4 * newMatrix[i%(num/2)][j%(num/2)] + 0;

                cout << (int)matrix[i][j] << " " << endl;
            }
        }
        count--;
        // cout << "\n";
    }
    // indexSize = index;
    matrixIndexing = matrix;

}

unsigned char **Dithering::ditheringMatrix(int a) {
    generateMatrix(ditheringImage, height, width);
    if(a == 0) {
        indexSize = 2;
        // matrixI2 = generateMatrix(indexSize, indexSize);
        generateIndexMatrix(indexSize);
        for(int i=0; i < height; i++) {
            for(int j=0; j < width; j++) {
                if(Image[i][j] > ((matrixIndexing[i%indexSize][j%indexSize] + 0.5)/ (indexSize*indexSize)) * 255) 
                    ditheringImage[i][j] = 255;
                else ditheringImage[i][j] = 0;
            }      
        }
        return ditheringImage;
    }
    
    if(a == 1) {
        indexSize = 8;
        // matrixI2 = generateMatrix(indexSize, indexSize);
        generateIndexMatrix(indexSize);
        for(int i=0; i < height; i++) {
            for(int j=0; j < width; j++) {
                if(Image[i][j] > ((matrixIndexing[i%indexSize][j%indexSize] + 0.5)/ (indexSize*indexSize)) * 255) 
                    ditheringImage[i][j] = 255;
                else ditheringImage[i][j] = 0;
            }      
        }
        return ditheringImage;
    }
    
    if(a == 2) {
        cout << "--------------------" << endl;
        indexSize = 32;
        // matrixI32 = generateMatrix(indexSize, indexSize);
        generateIndexMatrix(indexSize);
        for(int i=0; i < height; i++) {
            for(int j=0; j < width; j++) {
                if(Image[i][j] > ((matrixIndexing[i%indexSize][j%indexSize] + 0.5)/ (indexSize*indexSize)) * 255) 
                    ditheringImage[i][j] = 255;
                else ditheringImage[i][j] = 0;
            }      
        }
        return ditheringImage;
    }
}

Dithering::~Dithering() {

}

int main(int argc, char *argv[]) {
    int height{atoi(argv[5])}, width{atoi(argv[6])}, index{atoi(argv[7])};
    string file_input{argv[1]}, fileOutFixed{argv[2]}, fileOutRandom{argv[3]},fileOutDithering{argv[4]};
    Dithering *dithering = new Dithering{height, width, 0};
    unsigned char *Imagedata = new unsigned char[height*width];
    dithering->readImage(file_input, Imagedata);
   
    dithering->generateImage(Imagedata);
//    
    // dithering->randomThresholding();
    dithering->writeImage(fileOutFixed, dithering->fixedThresholding());
    dithering->writeImage(fileOutRandom, dithering->randomThresholding()); 
    dithering->writeImage(fileOutDithering, dithering->ditheringMatrix(index));

    delete dithering;
    delete Imagedata;
    return 0;
}

