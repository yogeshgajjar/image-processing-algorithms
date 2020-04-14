#include "dithering.h"
#include <iostream>
#include <vector> 
#include <cmath> 

using namespace std;

Dithering::Dithering(int h, int w, int i) 
    : height{h}, width{w}, indexSize{i} {

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

unsigned char **Dithering::generateMatrix(int h, int w) {
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
    fixedThresh = generateMatrix(height, width);
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            if(Image[i][j] < 127) fixedThresh[i][j] = 0;
            else fixedThresh[i][j] = 255;
        }
    }
    return fixedThresh;
}

unsigned char **Dithering::randomThresholding() {
    randomThresh = generateMatrix(height, width);
    unsigned char random{};
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) { 
            random = rand()%256;
            if(Image[i][j] > random) randomThresh[i][j] = 255;
            else randomThresh[i][j] = 0;
        }
    }
    return randomThresh;
}

void Dithering::generateIndexMatrix(int index) {
    unsigned char **matrix, **newMatrix;
    matrixIndexing = generateMatrix(index, index);
    int count{(log(index) / log(2))}, num{1}; 
    matrix = generateMatrix(num, num);  
    while(count > 0) {
        num *= 2;
        newMatrix = matrix;
        matrix = generateMatrix(num, num);
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
    }
    matrixIndexing = matrix;
}

unsigned char **Dithering::ditheringMatrix(int a) {
    ditheringImage = generateMatrix(height, width);
    if(a == 0) {
        indexSize = 2;
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


