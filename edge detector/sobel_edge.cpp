/*
Version of OS - Ubuntu 18.04.3 LTS
IDE - Visual Studio Code for C++
Compiler- g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0
Linker - g++
Image Viewing Software - ImageJ (http://rsb.info.nih.gov/ij/)

This code is belongs to Yogesh Gajjar
------------------------------------------------------------------------
                		Sobel Edge Detector
------------------------------------------------------------------------
ALGORITHM IMPLEMENTED
1.  Converted the RGB image to Grayscale using the formula provided in the assignment. 
2.  Boundary extension on all the sides. Used mirror reflection method. 
3.  Initialized the sobel operators and performed convolution on the boundary extended image to find x-gradient map, y-gradient map and gradient magnitude map. 
4.  Performed normalization by calculating the minimum and maximum values of x-gradient map, y-gradient map and total image map and used a image normalization formula to calculate the 
normalized outputs.  
5.  Calculated CDF and to find the threshold value. Selected a pixel value corresponding to 85%, 90%, 95% and 99% of the CDF value. Compared it with normalized gradient map. If found 
greater than threshold, map it to 0, else mapped it to 1.  
6. 	Write the image onto a .raw file 

STEPS TO RUN THE CODE 
1.  Open terminal and change the directory where the source code files are downloaded. 
2.  Compile the codes using: "g++ sourcecode.cpp -o program" 
3.  Run and execute the code and then write "./program input_image.raw gradient_x.raw gradient_y.raw gradient_map.raw normalized_image.raw BytePerPixel threshold" 
4.  Use the Image Viewing Software to see the results. 

*/ 

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath> 
#include <vector>
#include <unordered_map>

using namespace std;


int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int height{321}, width{481};
	
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 4){
		BytesPerPixel = 1; // default is grey image
	} 
	else {
		BytesPerPixel = atoi(argv[6]);
		// Check if size is specified
		
	}
	
	// Allocate image data array
	unsigned char Imagedata[height][width][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	// Boundary extension for convolution 
	unsigned char Imagedata_new[height+2][width+2][BytesPerPixel];
	for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
			for(int k=0; k < BytesPerPixel; k++) {
            	Imagedata_new[i+1][j+1][k] = Imagedata[i][j][k];
			}
        }
    }
	// for top
	for(int j=1; j < width+1; j++) {
		for(int k=0; k < BytesPerPixel; k++) {
			Imagedata_new[0][j][k] = Imagedata_new[1][j][k];
		}
	}
	// for left 
	for(int i=1; i < height+1; i++) {
		for(int k=0; k < BytesPerPixel; k++) {
			Imagedata_new[i][0][k] = Imagedata_new[i][1][k];
		}
	}
	// for bottom
	for(int j=1; j < width+1; j++) {
		for(int k=0; k < BytesPerPixel; k++) {
			Imagedata_new[height+1][j][k] = Imagedata_new[height][j][k];
		}
	}
	// for right 
	for(int i=1; i < height+1; i++) {
		for(int k=0; k < BytesPerPixel; k++) {
			Imagedata_new[i][width+1][k] = Imagedata_new[i][width][k];
		}
	}
	Imagedata_new[0][0][0] = Imagedata[0][0][0];
	Imagedata_new[0][width+2][0] = Imagedata[0][width][0];
	Imagedata_new[height+2][0][0] = Imagedata[height][0][0];
	Imagedata_new[height+2][width+2][0] = Imagedata[height][width][0];

	// Converting the 3-channel color image to 1-channel gray-scale image 
    unsigned char grayscale_image[height+2][width+2][1];
	unsigned char R_pixel[height+2][width+2][1], G_pixel[height+2][width+2][1], B_pixel[height+2][width+2][1];

	for(int i=0; i < height+2; i++) {
		for(int j=0; j < width+2; j++) {
			R_pixel[i][j][0] = Imagedata_new[i][j][0];
			G_pixel[i][j][0] = Imagedata_new[i][j][1];
			B_pixel[i][j][0] = Imagedata_new[i][j][2];
		}
	}

    for(int i=0; i < height+2; i++) {
        for(int j=0; j < width+2; j++) {
                grayscale_image[i][j][0] = (unsigned char) ((0.2989 * R_pixel[i][j][0]) + (0.5870 * G_pixel[i][j][0]) + (0.1140 * B_pixel[i][j][0]));
        }
    }

	// defining the sobel operators 
	float sobelX[3][3] = {{-1, 0, 1},
						  {-2, 0, 2},
						  {-1, 0, 1}};
	float sobelY[3][3] = {{1, 2, 1},
						  {0, 0, 0},
						  {-1, -2, -1}};

	signed int minX, maxX, minY, maxY, minGrad, maxGrad;
	int gX[height][width][1], gY[height][width][1];
	double sumgX{}, sumgY{}, sum{};
    double gfinal[height][width][1];
	unsigned char normalizedX[height][width][1], normalizedY[height][width][1], normalizedFinal[height][width][1];

	// Performing the convolution and normalizing the pixels of the image. 
	for(int i=0; i < height; i++) {
		for(int j=0; j < width; j++) {
			sumgX = 0;
			sumgY = 0;
			for(int k=0; k < 3; k++) {
				for(int l=0; l < 3; l++) {
					sumgX += (sobelX[k][l] * grayscale_image[i+k][j+l][0]);
					sumgY += (sobelY[k][l] * grayscale_image[i+k][j+l][0]);
				}
			}
			gX[i][j][0] = sumgX;
            gY[i][j][0] = sumgY;
            gfinal[i][j][0] = sqrt(sumgX*sumgX + sumgY*sumgY);

            if(maxX < gX[i][j][0]) maxX = gX[i][j][0];
			if(minX > gX[i][j][0]) minX = gX[i][j][0];
          
			if(maxY < gY[i][j][0]) maxY = gY[i][j][0];
			if(minY > gY[i][j][0]) minY = gY[i][j][0];

            if(maxGrad < gfinal[i][j][0]) maxGrad = gfinal[i][j][0];
			if(minGrad > gfinal[i][j][0]) minGrad = gfinal[i][j][0];
		}
	}	

	for(int i=0; i < height; i++) {
		for(int j=0; j < width; j++) {
			normalizedX[i][j][0] = 255 * (gX[i][j][0] - minX) / (maxX - minX);
            normalizedY[i][j][0] = 255 * (gY[i][j][0] - minY) / (maxY - minY);
            normalizedFinal[i][j][0] = 255 * (gfinal[i][j][0] - minGrad) / (maxGrad - minGrad); 
		}
	}	

	// Thresholding the image based on the threshold value
	double histArray[256]{};
	for(int i=0; i < height; i++) {
		for(int j=0; j < width; j++) {
			histArray[normalizedFinal[i][j][0]]++;
		}
	}

	double histArrayNormalized[256]{};
	for(int i=0; i < 256; i++) {
		histArrayNormalized[i] = histArray[i]/(height*width);
	}

	double cdf[256]{};
	cdf[0] = histArrayNormalized[0];
	for(int i=1; i < 256; i++) {
		cdf[i] = cdf[i-1] + histArrayNormalized[i];
	}

	int thresholdImageValue{};
	float cutoff{atof(argv[7])};
	vector<int> temp;
	for(int i=0; i < 256; i++) {
		if(cdf[i] > cutoff) {
			temp.push_back(i);
		}
	}
	
	thresholdImageValue = temp[0];
		  
    unsigned char finalThreshold[height][width][1];
	for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            if(normalizedFinal[i][j][0] > thresholdImageValue) finalThreshold[i][j][0] = 0;
            else finalThreshold[i][j][0] = 255;
        }
    }

	// Write image data 	(filename specified by second argument) from image data matrix
	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(normalizedX, sizeof(unsigned char), height*width, file);
	fclose(file);

    if (!(file=fopen(argv[3],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(normalizedY, sizeof(unsigned char), height*width, file);
	fclose(file);

    if (!(file=fopen(argv[4],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	cout << height << " " << width << " " << BytesPerPixel << endl;
	fwrite(normalizedFinal, sizeof(unsigned char), height*width, file);
	fclose(file);

    if (!(file=fopen(argv[5],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(finalThreshold, sizeof(unsigned char), height*width, file);
	fclose(file);

	return 0;
}
