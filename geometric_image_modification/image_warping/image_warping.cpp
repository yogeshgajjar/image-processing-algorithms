/*
Version of OS - Ubuntu 18.04.3 LTS
IDE - Visual Studio Code for C++
Compiler- g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Linker - g++
Image Viewing Software - ImageJ (http://rsb.info.nih.gov/ij/)

Copyright © 2020 Yogesh Gajjar. All rights reserved.

*/

#include "image_warping.h"
#include <iostream>
#include <cmath>
#include <string>

using namespace std; 


Warping::Warping(int h, int w, int b) {
    height = h;
    width = w;
    BytesPerPixel = b;
    u = v = x = y = 0.0;
}


void Warping::readImage(string fileName, unsigned char *image) {
    FILE *file;
    if (!(file=fopen(fileName.data(),"rb"))) {
		cout << "Cannot open file: " << fileName.data() <<endl; 
		exit(1);
	}

	fread(image, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);
}

/* Method to write the image into image format */ 
void Warping::writeImage(string filename, unsigned char ***final) {
    FILE *file;

    unsigned char finalImg[height*width*BytesPerPixel];
    long int count{};
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            for(int k=0; k < BytesPerPixel; k++) {
                finalImg[count] = final[i][j][k];
                count++;
            }
        }
    }

    if (!(file=fopen(filename.data(),"wb"))) {
		cout << "Cannot open file: " << filename.data() << endl;
		exit(1);
	}
	fwrite(finalImg, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

}

void Warping::generateImage(unsigned char *main_image) {
    long int count{};
    Image = new unsigned char **[height];
    for(int i = 0; i < height; i++) {
        Image[i] = new unsigned char *[width];
        for(int j = 0; j < width; j++) {
            Image[i][j] = new unsigned char [BytesPerPixel];
            for(int k=0; k < BytesPerPixel; k++) {
                Image[i][j][k] = main_image[count];
                count++;
            }
        }
    }
}

unsigned char ***Warping::generateMatrix3D(int h, int w, int b) {
    long int count{};
    unsigned char ***image = new unsigned char **[h];
    for(int i=0; i < h; i++) {
        image[i] = new unsigned char *[w];
        for(int j=0; j < w; j++) {
            image[i][j] = new unsigned char [b];
            for(int k=0; k < b; k++) {
                image[i][j][k] = 0;
                count++;
            }
        }
    }

    return image;
}

int Warping::signum(double x) {
    return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
}

unsigned char ***Warping::diskWarping() {
    unsigned char ***image;
    image = generateMatrix3D(height, width, BytesPerPixel);
    imageWarp = generateMatrix3D(height, width, BytesPerPixel);
    imageorig = generateMatrix3D(height, width, BytesPerPixel);
    image = Image;
    double oldmin{}, oldmax{height}, newmax{1}, newmin{-1};
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            for(int k=0; k < BytesPerPixel; k++) {
                x = ((i - oldmin) * ((newmax - newmin) / (oldmax - oldmin))) + newmin; 
                y = ((j - oldmin) * ((newmax - newmin) / (oldmax - oldmin))) + newmin;
                u = (x * (sqrt(1 - (y * y)/2))+1)*(height/2);
                v = (y * (sqrt(1 - (x * x)/2))+1)*(height/2);
                imageWarp[(int)u][(int)v][k] = image[i][j][k];           
            }
        }
    }
    return imageWarp; 
}



unsigned char ***Warping::planeWarping() {
    unsigned char ***image;
    double new_x{}, new_y{};
    image = generateMatrix3D(height, width, BytesPerPixel);
    imageorig = generateMatrix3D(height, width, BytesPerPixel);
    image = Image;
    double oldmin{}, oldmax{height}, newmax{1}, newmin{-1};

    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            for(int k=0; k < BytesPerPixel; k++) {
                x = ((i - oldmin) * ((newmax - newmin) / (oldmax - oldmin))) + newmin; 
                y = ((j - oldmin) * ((newmax - newmin) / (oldmax - oldmin))) + newmin;
                               
                u = (x * (sqrt(1 - (y * y)/2)));
                v = (y * (sqrt(1 - (x * x)/2)));
                
                if (i == 0 && j == 0) {
                    new_x = 0;
                    new_y = 0;
                    continue;
                }

                x = ((0.5 * (sqrt(2 + (u * u) - (v * v) + 2 * sqrt(2) * u))) - (0.5 * (sqrt(2 + (u * u) - (v * v) - 2 * sqrt (2) * u))));
                y = ((0.5 * (sqrt(2 - (u * u) + (v * v) + 2 * sqrt(2) * v))) - (0.5 * (sqrt(2 - (u * u) + (v * v) - 2 * sqrt (2) * v))));
                double new_x = (x + 1)*(height/2);
                double new_y = (y + 1)*(height/2);
                imageorig[i][j][k] = image[(int)new_x][(int)new_y][k];                
            }   
        }
    }
    
    return imageorig; 
}

void Warping::delete3D(unsigned char ***image, int h, int w) {
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
			delete[] image[i][j];
		delete[] image[i];
	}
	delete[] image;
}
