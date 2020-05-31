/*.
Copyright © 2020 Yogesh Gajjar. All rights reserved.
*/


#include "error_diffusion.h"
#include <iostream>
#include <string>

using namespace std;



ErrorDiffusion::ErrorDiffusion (int h, int w, long double e) 
    : height{h}, width{w}, error{e} {
}

ErrorDiffusion::~ErrorDiffusion() {
}

void ErrorDiffusion::readImage(string fileName, unsigned char *image) {
    FILE *file;
    if (!(file=fopen(fileName.data(),"rb"))) {
		cout << "Cannot open file: " << fileName.data() <<endl; 
		exit(1);
	}

	fread(image, sizeof(unsigned char), height*width, file);
	fclose(file);
}

/* Method to write the image into image format */ 
void ErrorDiffusion::writeImage(string filename, unsigned char **final) {
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

unsigned char **ErrorDiffusion::generateMatrix(int h, int w) {
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

void ErrorDiffusion::generateImage(unsigned char *main_image) {
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

unsigned char **ErrorDiffusion::boundaryExtension(int pad) {
    ImagewithBoundary = generateMatrix((2*pad)+height, (2*pad)+width);

    long int count{};
    for(int i=0; i < height; i++) {
        for(int j=0; j < width; j++) {
            ImagewithBoundary[i+pad][j+pad] = Image[i][j];
            count++;
        }
    }
    return ImagewithBoundary;
}

unsigned char **ErrorDiffusion::errorFS() {
    window = 3;
    padding = window/2;

    float FS[3][3] = {{0,0,0}, 
                      {0,0,7.0},
                      {3.0,5.0,1.0}};
    
    imageTemp = boundaryExtension(padding);
    imageTempBinarized = generateMatrix(height, width);
    
    for (int i = padding; i < height+padding; i++ ) {
        if((i-padding)%2 == 0) {
            for (int j = padding; j < width+padding; j++) {
                if (imageTemp[i][j] < 128) {
                    error = imageTemp[i][j] - 0;
                    imageTempBinarized[i-padding][j-padding] = 0;
                }
                else {
                    error = imageTemp[i][j] - 255;
                    imageTempBinarized[i-padding][j-padding] = 255;
                }
                for (int k = 0; k < window; k++) {
                    for (int l = 0; l < window; l++) {
                        imageTemp[i-padding+k][j-padding+l] += (unsigned char)(error * (FS[k][l]/16.0));
                    }
                }
            }
        }

        if((i-padding)%2 !=0) {
            for (int j = width; j > 0; j--) {
                if (imageTemp[i][j] < 128) {
                    error = imageTemp[i][j] - 0;
                    imageTempBinarized[i-padding][j-padding] = 0;
                }
                else {
                    error = imageTemp[i][j] - 255;
                    imageTempBinarized[i-padding][j-padding] = 255;
                }
                for (int k = 0; k < window; k++) {
                    for (int l = window-1; l >=0; l--) {
                        imageTemp[i-padding+k][j-padding+l] += (unsigned char)(error * (FS[k][l]/16.0));
                    }
                }
            }
        }
        
    }

    return imageTempBinarized;
    
}

unsigned char **ErrorDiffusion::errorJJN() {
    window = 5;
    padding = window/2;

    float JJN[5][5] = {{0,0,0,0,0},
                       {0,0,0,0,0},
                       {0,0,0,7.0,5.0},
                       {3.0,5.0,7.0,5.0,3.0},
                       {1.0,3.0,5.0,3.0,1.0}};

    imageTemp = boundaryExtension(padding);
    imageTempBinarized = generateMatrix(height, width);

    for(int i=padding; i < height+padding; i++) {
        for(int j=padding; j < width+padding; j++) {
            if(imageTemp[i][j] > 127) {
                error = imageTemp[i][j] - 255;
                imageTempBinarized[i-padding][j-padding] = 255;
            }
            else {
                error = imageTemp[i][j] - 0;
                imageTempBinarized[i-padding][j-padding] = 0;
            }
            for(int k=0; k < window; k++) {
                for(int l=0; l < window; l++) {
                    imageTemp[i-padding+k][j-padding+l] += (unsigned char)(error * (JJN[k][l]/48.0));
                }
            }
        }
    }
    return imageTempBinarized;
}

unsigned char **ErrorDiffusion::errorS() {
    window = 5;
    padding = window/2;

    float S[5][5] = {{0,0,0,0,0},
                       {0,0,0,0,0},
                       {0,0,0,8.0,4.0},
                       {2.0,4.0,8.0,4.0,2.0},
                       {1.0,2.0,4.0,2.0,1.0}};

    imageTemp = boundaryExtension(padding);
    imageTempBinarized = generateMatrix(height, width);

    for(int i=padding; i < height+padding; i++) {
        for(int j=padding; j < width+padding; j++) {
            if(imageTemp[i][j] > 127) {
                error = imageTemp[i][j] - 255;
                imageTempBinarized[i-padding][j-padding] = 255;
            }
            else {
                error = imageTemp[i][j] - 0;
                imageTempBinarized[i-padding][j-padding] = 0;
            }
            for(int k=0; k < window; k++) {
                for(int l=0; l < window; l++) {
                    imageTemp[i-padding+k][j-padding+l] += (unsigned char)(error * (S[k][l]/42.0));
                }
            }
        }
    }
    return imageTempBinarized;
}